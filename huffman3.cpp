#include <iostream>
#include <cstdlib>
#include <queue>
#include <functional>
#include <unordered_map>
#include <string>
#include <streambuf>
#include <sstream>
#include <bitset>


using namespace std;
#define MAX 1000
char inputFile[20], outputFile[20];
int fileSize, nodesSize;
string output;
vector<char>file;
vector<char> file_code;
vector<char>comp_file;
unordered_map<char, string> code_table;
unordered_map<char, int> freq_table;


typedef struct Node
{
public:
    char character;
    int freq = 0;
    Node* left, * right;


} Node;

struct Comparefreq
{
    bool operator()(Node* n1, Node* n2)
    {
        return n1->freq > n2->freq;
    }
};

priority_queue<Node*, std::vector<Node*>, Comparefreq > q;

bool getInputs()
{
    bool choice;
    printf("please enter input file name: ");
    scanf("%s", &inputFile);
    printf("to compress file press 1\nto decompress file press 2\n");
    cin >> choice;
    return choice;

}

void readFile()
{
    FILE* fptr = fopen(inputFile, "r");
    int i = 0;
    char x;
    int freq = 0;
    while (!feof(fptr))
    {

        fscanf(fptr, "%c", &x);
        file.push_back(x);
        auto it = freq_table.find(x);
        if (it != freq_table.end())
            it->second++;
        else
            freq_table[x] = 1;
        i++;
    }
    fclose(fptr);

}


void createNodes()
{

    for (auto x : freq_table)
    {

        Node* temp = (Node*)malloc(sizeof(Node));
        temp->character = x.first;
        temp->freq = x.second;
        temp->left = NULL;
        temp->right = NULL;
        q.push(temp);

    }


}

void huffmantree()
{
    while (q.size() != 1)
    {
        Node* newnode = (Node*)malloc(sizeof(Node));
        newnode->left = q.top();
        int freq = q.top()->freq;
        q.pop();
        newnode->right = q.top();
        newnode->freq = freq + q.top()->freq;
        q.pop();
        q.push(newnode);

    }


}



bool isleaf(Node* root)
{
    if (root->left == NULL && root->right == NULL)
        return true;
    else;
    return false;
}

void traversetree(Node* root, string code)
{
    if (root == NULL)
        return;
    traversetree(root->left, code + '0');
    if (isleaf(root))
        code_table[root->character] = code;
    traversetree(root->right, code + '1');
}

void encode()

{
    string h;
    for (int i = 0; i < file.size() - 1; i++)
    {
        h += code_table[file[i]];
    }
    stringstream Stream(h);
    while (Stream.good())
    {
        bitset<8> bits;
        Stream >> bits;
        char c = char(bits.to_ulong());
        output += c;

    }

}
void write_file()
{
    FILE* fptr = fopen("output_file.txt", "w");

    if (fptr == nullptr)
        return;
    cout << "Coding Table [ Character , Code ] " << endl;
    for (auto x : code_table)
    {
        fprintf(fptr, "%c", x.first);
        for (int i = 0; i < x.second.size(); i++)
            fprintf(fptr, "%c", x.second[i]);
        fprintf(fptr, "\n");
        cout << x.first << "," << x.second << " " << endl;
    }
    fprintf(fptr, "====\n");

    for (int i = 0; i < output.size(); i++)
        fprintf(fptr, "%c", output[i]);
    cout << "\n================\nASCII code :" << output;

    /*freopen("output_file.txt", "w", stdout);
    for (auto x : code_table)
        cout << x.first << "," << x.second << " " << endl;
    cout << "Á" << endl;
    for (int j = 0;j < comp_file.size();j++)
        cout << comp_file[j];*/
}

void readcompfile()
{
    char x{}, dumb;
    char s[20];
    FILE* fptr=fopen("output_file.txt", "r");
    while (x!='=')
    {
        fscanf(fptr,"%c", &x);
        fscanf(fptr, "%s", s);
        fscanf(fptr, "%c", &dumb);
        if (s != "===")
        code_table[x] = s;
        else
            break;
    }
    for (auto x : code_table)
        cout << x.first << "," << x.second << " " << endl;
    //FILE* alo = fopen("output_file.txt", "r");

    while (!feof(fptr))
    {
        fscanf(fptr, "%c", &x);
        //cout << x;
        file.push_back(x);

    }
    for (int i = 0; i < file.size(); i++)
    {
        cout << file[i];

    }
        
    
    


}

void compress()
{
    readFile();
    createNodes();
    huffmantree();
    traversetree(q.top(), "");
    encode();
    write_file();
}

void decompress()
{
    readcompfile();

}

int main()
{
    if (getInputs())
        compress();
    else
        decompress();


    return 0;
}
