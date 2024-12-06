#include <argparse.h>
void get_opts(int argc,
              char **argv,
              struct options_t *opts)
{
    if (argc == 1)
    {
        std::cout << "Usage:" << std::endl;
        std::cout << "\t--inputfilename or -i <file_path>" << std::endl;
        std::cout << "\t--outputfilename or -o <file_path>" << std::endl;
        std::cout << "\t--steps or -s <steps>" << std::endl;
        std::cout << "\t--theta or -t <theta>" << std::endl;
        std::cout << "\t--dt or -d <timestep>" << std::endl;
        std::cout << "\t[Optional] --visual or -V" << std::endl;
        exit(0);
    }

    opts->visual = false;

    struct option l_opts[] = {
        {"inputfilename", required_argument, NULL, 'i'},
        {"outputfilename", required_argument, NULL, 'o'},
        {"steps", required_argument, NULL, 's'},
        {"theta", required_argument, NULL, 't'},
        {"dt", required_argument, NULL, 'd'},
        {"visual", no_argument, NULL, 'V'}
    };

    int ind, c;
    while ((c = getopt_long(argc, argv, "i:o:s:t:d:V", l_opts, &ind)) != -1)
    {
        switch (c)
        {
        case 0:
            break;
        case 'i':
            opts->inputfilename = (char *)optarg;
            break;
        case 'o':
            opts->outputfilename = (char *)optarg;
            break;
        case 's':
            opts->steps = atoi((char *)optarg);
            break;
        case 't':
            opts->theta = atof((char *)optarg);
            break;
        case 'd':
            opts->timestep = atof((char *)optarg);
            break;
        case 'V':
            opts->visual = true;
            break;
        case ':':
            std::cerr << argv[0] << ": option -" << (char)optopt << "requires an argument." << std::endl;
            exit(1);
        }
    }
}
