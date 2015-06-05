
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "server.h"


static void parse_options(uttp_server_config_t *config, int argc, char** argv) {
    for (;;) {
        static struct option uttp_options[] = {
            {"interface", required_argument, 0, 'i'},
            {"port",      required_argument, 0, 'p'},
            {"workers",   required_argument, 0, 'w'},
            {"help",      required_argument, 0, 'h'},
            {0, 0, 0, 0}
        };
        int c;
        int index = 0;

        c = getopt_long(argc, argv, "hi:p:w:", uttp_options, &index);
        if (c == -1) {
            break;
        }

        switch (c) {
            case 'i':
                config->interface = optarg;
                break;
            case 'p':
                if (sscanf(optarg, "%hu", &config->port) != 1) {
                    fprintf(stderr, "Invalid port specifiedi: %s\n", optarg);
                    exit(1);
                }
                break;
            case 'w':
                if (sscanf(optarg, "%u", &config->nworkers) != 1) {
                    fprintf(stderr, "Invalid amount of workers specifiedi: %s\n", optarg);
                    exit(1);
                }
                break;
            default:
                fprintf(stderr,
                        "Usage:\n"
                        "\n"
                        "  %s [options]\n"
                        "\n"
                        "Options:\n"
                        "\n"
                        "  -i, --interface <address>  Bind to this address. Default: \"0.0.0.0\"\n"
                        "  -p, --port <port>          Bind to this port number. Default: 0\n"
                        "  -h                         Show this help message.\n"
                        "",
                        argv[0]);
        }
    }

}


int main(int argc, char** argv) {
    int r;
    uttp_server_t server;
    uttp_server_config_t config;
    memset(&config, 0, sizeof(uttp_server_config_t));
    parse_options(&config, argc, argv);

    r = uttp_server_run(&server, &config, NULL);
    uttp_server_close(&server);
    return r;
}
