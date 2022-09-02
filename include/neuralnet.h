#ifndef SRC_NEURALNET_H
#define SRC_NEURALNET_H

typedef struct neuron_t Neuron;
typedef struct layer_t Layer;

typedef struct network_t {
    int layerc;
    Layer **layers;
    double (*activation_func)(double);
} Network;

Network *create_network(int* layersizes, int layerc, double (*activation_func)(double));
void init_random(Network *net, double w, double b);
void calc_network(Network *net);
void free_network(Network *net);
void set_network_input(Network *net, double *values, int valuec, int start_neuron);
int get_network_output(Network *net, double **values);
int get_network_size(Network *net, int **layersizes);
Network *add_networks(Network *net1, Network *net2);
void mult_network(Network *net, double x);

#endif