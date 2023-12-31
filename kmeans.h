#ifndef KMEANS_H
#define KMEANS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct KMeansResult {
    int* labels;
    double* cluster_centers;
    double inertia;
};

struct KMeansResult kmeans_fit(double* X, int n_samples, int n_features, int n_clusters) {
    struct KMeansResult result;
    result.labels = (int*)malloc(n_samples * sizeof(int));
    result.cluster_centers = (double*)malloc(n_clusters * n_features * sizeof(double));
    result.inertia = 0.0;

    // K-means clustering algorithm implementation
    // Initialize cluster centers randomly
    for (int i = 0; i < n_clusters; i++) {
        for (int j = 0; j < n_features; j++) {
            result.cluster_centers[i * n_features + j] = X[rand() % (n_samples * n_features) + j];
        }
    }

    int max_iterations = 100;  // Maximum number of iterations
    double tolerance = 1e-4;   // Convergence tolerance
    double prev_inertia = INFINITY;

    double inertia; // Declare inertia here

    for (int iter = 0; iter < max_iterations; iter++) {
        // Assign samples to the nearest cluster center
        for (int i = 0; i < n_samples; i++) {
            int closest_cluster = 0;
            double min_distance = INFINITY;

            for (int j = 0; j < n_clusters; j++) {
                double distance = 0.0;
                for (int k = 0; k < n_features; k++) {
                    double diff = X[i * n_features + k] - result.cluster_centers[j * n_features + k];
                    distance += diff * diff;
                }

                if (distance < min_distance) {
                    min_distance = distance;
                    closest_cluster = j;
                }
            }

            result.labels[i] = closest_cluster;
        }

        // Update cluster centers
        double* cluster_sums = (double*)calloc(n_clusters * n_features, sizeof(double));
        int* cluster_counts = (int*)calloc(n_clusters, sizeof(int));

        for (int i = 0; i < n_samples; i++) {
            int cluster = result.labels[i];
            cluster_counts[cluster]++;

            for (int j = 0; j < n_features; j++) {
                cluster_sums[cluster * n_features + j] += X[i * n_features + j];
            }
        }

        for (int i = 0; i < n_clusters; i++) {
            if (cluster_counts[i] > 0) {
                for (int j = 0; j < n_features; j++) {
                    result.cluster_centers[i * n_features + j] = cluster_sums[i * n_features + j] / cluster_counts[i];
                }
            }
        }

        free(cluster_sums);
        free(cluster_counts);

        // Compute the sum of squared distances to nearest cluster center (inertia)
        inertia = 0.0;
        for (int i = 0; i < n_samples; i++) {
            int cluster = result.labels[i];
            for (int j = 0; j < n_features; j++) {
                double diff = X[i * n_features + j] - result.cluster_centers[cluster * n_features + j];
                inertia += diff * diff;
            }
        }

        // Check convergence
        if (fabs(inertia - prev_inertia) < tolerance){            
        break;
        }
        prev_inertia = inertia;
    }

    result.inertia = inertia;

    return result;
}


int* kmeans_predict(double* X, int n_samples, int n_features, double* cluster_centers, int n_clusters) {
    int* labels = (int*)malloc(n_samples * sizeof(int));

    // Predict cluster labels based on the trained model
    for (int i = 0; i < n_samples; i++) {
        int closest_cluster = 0;
        double min_distance = INFINITY;

        for (int j = 0; j < n_clusters; j++) {
            double distance = 0.0;
            for (int k = 0; k < n_features; k++) {
                double diff = X[i * n_features + k] - cluster_centers[j * n_features + k];
                distance += diff * diff;
            }

            if (distance < min_distance) {
                min_distance = distance;
                closest_cluster = j;
            }
        }

        labels[i] = closest_cluster;
    }

    return labels;
}

double* kmeans_transform(double* X, int n_samples, int n_features, double* cluster_centers, int n_clusters) {
    double* distances = (double*)malloc(n_samples * n_clusters * sizeof(double));

    // Compute distances to cluster centers
    for (int i = 0; i < n_samples; i++) {
        for (int j = 0; j < n_clusters; j++) {
            double distance = 0.0;
            for (int k = 0; k < n_features; k++) {
                double diff = X[i * n_features + k] - cluster_centers[j * n_features + k];
                distance += diff * diff;
            }
            distances[i * n_clusters + j] = sqrt(distance);
        }
    }

    return distances;
}

double kmeans_score(double* X, int n_samples, int n_features, double* cluster_centers, int n_clusters) {
    double inertia = 0.0;

    // Compute the sum of squared distances to nearest cluster center
    for (int i = 0; i < n_samples; i++) {
        int closest_cluster = 0;
        double min_distance = INFINITY;

        for (int j = 0; j < n_clusters; j++) {
            double distance = 0.0;
            for (int k = 0; k < n_features; k++) {
                double diff = X[i * n_features + k] - cluster_centers[j * n_features + k];
                distance += diff * diff;
            }

            if (distance < min_distance) {
                min_distance = distance;
            }
        }

        inertia += min_distance;
    }

    return inertia;
}

void free_kmeans_result(struct KMeansResult result) {
    free(result.labels);
    free(result.cluster_centers);
}

#endif  // KMEANS_H
