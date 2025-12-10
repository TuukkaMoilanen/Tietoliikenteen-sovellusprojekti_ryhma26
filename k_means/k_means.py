import numpy as np
import matplotlib.pyplot as plt

def create_test_data():
    """
    Luo testidata kiihtyvyysanturin tyypillisistä x,y,z-arvoista.
    Mukana 6 suuntaa: x+, x-, y+, y-, z+, z-
    Viimeinen sarake on oikea luokka (label), jota K-means EI käytä.
    """

    np.random.seed(0)

    data = np.zeros((600, 4))  # 600 riviä, 4 saraketta (x,y,z,label)

    data[0:100, :]   = np.array([1800,1500,1500,0])   # x+
    data[100:200, :] = np.array([1200,1500,1500,1])   # x-
    data[200:300, :] = np.array([1500,1800,1500,2])   # y+
    data[300:400, :] = np.array([1500,1200,1500,3])   # y-
    data[400:500, :] = np.array([1500,1500,1800,4])   # z+
    data[500:600, :] = np.array([1500,1500,1200,5])   # z-
    data[:,0:3] = data[:,0:3] + 70 * np.random.rand(600, 3)

    return data

"""piirretään 3D kuva testidatalle"""
def plot_3d_data(data):
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.scatter(data[:,0], data[:,1], data[:,2], s=10)
    ax.set_xlabel("X")
    ax.set_ylabel("Y")
    ax.set_zlabel("Z")
    plt.title("Testidata 3D-kuvana")
    plt.show()

"""Lasketaan kahden 3D pisteen etäisyys """
def euclidean_distance(p1, p2):
    return np.linalg.norm(p1 - p2)

"""Arvotaan 6satunnaista keskipistettä"""
def initialize_centroids(data, K=6):
    min_x = np.min(data[:, 0])
    max_x = np.max(data[:, 0])
    min_y = np.min(data[:, 1])
    max_y = np.max(data[:, 1])
    min_z = np.min(data[:, 2])
    max_z = np.max(data[:, 2])

    centroids = np.zeros((K, 3))

    for i in range(K):
        centroids[i, 0] = np.random.uniform(min_x, max_x)
        centroids[i, 1] = np.random.uniform(min_y, max_y)
        centroids[i, 2] = np.random.uniform(min_z, max_z)

    return centroids

def sort_centroids_for_accelerometer(centroids):
    sorted_list = [None] * 6

    x_min = np.argmin(centroids[:,0])   # x-
    x_max = np.argmax(centroids[:,0])   # x+
    y_min = np.argmin(centroids[:,1])   # y-
    y_max = np.argmax(centroids[:,1])   # y+
    z_min = np.argmin(centroids[:,2])   # z-
    z_max = np.argmax(centroids[:,2])   # z+

    sorted_list[0] = centroids[z_max]
    sorted_list[1] = centroids[z_min]
    sorted_list[2] = centroids[y_max]
    sorted_list[3] = centroids[y_min]
    sorted_list[4] = centroids[x_max]
    sorted_list[5] = centroids[x_min]

    return np.array(sorted_list)

"""Etsitään jokaisen datapisteen lähin keskipiste"""
def step1_assign(data, centroids):
    K = 6

    centerPointCumulativeSum = np.zeros((K,3))
    counts = np.zeros(K)

    for point in data[:,0:3]:

        distances = np.zeros(K)
        for c in range(K):
            distances[c] = euclidean_distance(point, centroids[c])
        winner = np.argmin(distances)
        counts[winner] += 1
        centerPointCumulativeSum[winner] += point

    return centerPointCumulativeSum, counts


"""Laskee uudet keskipisteet tai arvotaan uudet jos ei voittanut step1 yhtään datapistettä """
def step2_update_centroids(cumulativeSum, counts, centroids):
    K = 6

    for c in range(K):
        if counts[c] > 0:
            centroids[c] = cumulativeSum[c] / counts[c]
        else:
            centroids[c] = np.random.rand(3) * np.max(centroids)

    return centroids

"""kmeans opetus"""
def kmeans_train(data, max_iters=50):
    centroids = initialize_centroids(data)

    for _ in range(max_iters):

        cumulativeSum, counts = step1_assign(data, centroids)
        new_centroids = step2_update_centroids(cumulativeSum, counts, centroids.copy())

        if np.allclose(centroids, new_centroids):
            break

        centroids = new_centroids

    return centroids


"""Selvittää mihin keskipisteeseen jokainen datapiste lopulta kuuluu"""
def assign_points_to_clusters(data, centroids):
    K = centroids.shape[0]
    assignments = np.zeros(len(data), dtype=int)

    for i, point in enumerate(data):
        distances = np.zeros(K)
        for c in range(K):
            distances[c] = euclidean_distance(point, centroids[c])
        assignments[i] = np.argmin(distances)

    return assignments

"""Kirjoitetaan lopulliset arvot C-taulukkoon"""
def write_centroids_to_header(centroids, filename="keskipisteet.h"):
    with open(filename, "w") as f:
        K = centroids.shape[0]
        f.write(f"int CP[{K}][3] = {{\n")
        for i in range(K):
            x, y, z = centroids[i]
            f.write(f"    {{{int(x)}, {int(y)}, {int(z)}}}")
            if i < K - 1:
                f.write(",\n")
            else:
                f.write("\n")
        f.write("};\n")


"""Piirtää tulokset 3Dtaulukkoon"""
def plot_clusters_and_centroids(data, assignments, centroids):
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')

    K = centroids.shape[0]
    names = ["x+", "x-", "y+", "y-", "z+", "z-"]  # Datapisteille oikeat nimet

    for k in range(K):
        cluster_points = data[assignments == k]
        ax.scatter(
            cluster_points[:,0],
            cluster_points[:,1],
            cluster_points[:,2],
            s=10,
            label=names[k]
        )

    ax.scatter(
        centroids[:,0], centroids[:,1], centroids[:,2],
        s=200, c='black', marker='X', label='keskipisteet'
    )

    ax.set_xlabel("X")
    ax.set_ylabel("Y")
    ax.set_zlabel("Z")
    plt.title("Datapisteet ja lopulliset keskipisteet")
    plt.legend()
    plt.show()

"""main ohjelma"""
if __name__ == "__main__":

    """Luodaan testidata"""
    data = create_test_data()
    xyz = data[:, 0:3]

    """Piirretään testidata 3D kuvaan"""
    plot_3d_data(data)

    """Kmeans opetus"""
    print("Aloitetaan k-means opetus…")
    centroids = kmeans_train(xyz)

    """Järjestetään pisteet"""
    centroids = sort_centroids_for_accelerometer(centroids)

    """C tiedosto"""
    write_centroids_to_header(centroids)
    print("keskipisteet.h luotu!")

    """Piirretään lopputulos"""
    assignments = assign_points_to_clusters(xyz, centroids)
    plot_clusters_and_centroids(xyz, assignments, centroids)
