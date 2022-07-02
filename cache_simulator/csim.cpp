//Teaju Kwon
//CS180
//Cache simulator
//2021 & Fall

#include "cachelab.h"
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

using namespace std;

typedef struct
{
    bool valid{ false };
    int tag;
    int timestamp{ 0 };
} cache_line;

struct Option
{
    bool valid{ false };
    int s{ 0 };
    int E{ 0 };
    int b{ 0 };
};

void print_help_info(void) {
    // Print the help inforation of this program
    printf("Usage: ./csim [-hv] -s <num> -E <num> -b <num> -t <file>\n");
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of set index bits.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of block offset bits.\n");
    printf("  -t <file>  Trace file.\n");
    printf("\n");
    printf("Examples:\n");
    printf("  linux>  ./csim -s 4 -E 1 -b 4 -t traces/yi.trace\n");
    printf("  linux>  ./csim -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");
    exit(EXIT_FAILURE);
}

vector<vector<cache_line>> cache;
int hit = 0;
int miss = 0;
int evic = 0;

void cache_lookup(int set_index, int Tag)
{
    for (auto& i : cache[set_index])
    {
        if (i.valid == true)
        {
            i.timestamp++;
        }
    }

    for (auto& i : cache[set_index])
    {
        if (i.tag == Tag && i.valid == true)
        {
            hit++;
            i.timestamp = 0;
            return;
        }
    }
    for (auto& i : cache[set_index])
    {
        if (i.valid == false)
        {
            miss++;
            i.valid = true;
            i.tag = Tag;
            i.timestamp = 0;
            return;
        }
    }

    int max = 0;
    int old_timestamp = 0;
    for (size_t i = 0; i < cache[set_index].size(); i++)
    {
        if (cache[set_index][i].timestamp > max)
        {
            max = cache[set_index][i].timestamp;
            old_timestamp = i;
        }
    }

    cache[set_index][old_timestamp].tag = Tag;
    cache[set_index][old_timestamp].valid = true;
    cache[set_index][old_timestamp].timestamp = 0;

    evic++;
    miss++;
}
int main(int argc, char* argv[])
{
    Option option;
    string file;

    int num;

    while ((num = getopt(argc, argv, "s:E:b:t:")) != -1)
    {
        switch (num)
        {
        case 'h':
            print_help_info();
            break;
        case 'v':
            option.valid = true;
            break;
        case 's':
            option.s = atoi(optarg);
            break;
        case 'E':
            option.E = atoi(optarg);
            break;
        case 'b':
            option.b = atoi(optarg);
            break;
        case 't':
            file = optarg;
            break;
        case '?':
            print_help_info();
            break;
        };
    }

    char type{ '\0' };
    long long addr{ 0 };
    string ignore{};

    ifstream fin{ file };

    int S = 1 << option.s;
    for (int i{ 0 }; i < S; i++)
        cache.push_back({});

    for (int i{ 0 }; i < S; i++)
        for (int j{ 0 }; j < option.E; j++)
            cache[i].push_back({});

    while (fin >> type >> hex >> addr >> ignore)
    {
        if (type == 'I')
        {
            continue;
        }
        else
        {
            addr >>= option.b;
            int set_mask = (1 << option.s) - 1;
            int set_index = addr & set_mask;
            addr >>= option.s;
            int tag = addr;

            if (type == 'L' || type == 'S')
            {
                cache_lookup(set_index, tag);
            }
            else if (type == 'M')
            {
                cache_lookup(set_index, tag);
                cache_lookup(set_index, tag);
            }
        }
    }

    //cout << type << " " << hex << addr << ignore << '\n';
    printSummary(hit, miss, evic);

}
