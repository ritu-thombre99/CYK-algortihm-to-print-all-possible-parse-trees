#include<iostream>
#include<bits/stdc++.h>
#include<string.h>
using namespace std;
string getString(char x) 
{ 
    string s(1, x); 
    return s;    
} 
typedef struct node
{
	char data;
	struct node *left;
	struct node *right;
	bool leaf = false;
	char terminal = '\0';
} Node;
int height_of(Node *root)
{
	if(root == NULL)
		return 0;
	else
	{
		int left = height_of(root->left);
		int right = height_of(root->right);
		if(left > right)
			return 1+left;
		else
			return 1+right;
	}
}
void printLevel(Node *root,int level,FILE *fptr)
{
	if(root != NULL)
	{
		if(level == 1)
		{	
			if(root->leaf == true)
			{
				fprintf(fptr,"%c->%c\t",root->data,root->terminal);
			}
			else
			{
				fprintf(fptr,"%c ->",root->data);
				string str = getString(root->left->data) + getString(root->right->data);
				fprintf(fptr,"%s\t",str.c_str());
			}
		}
		else
		{
			printLevel(root->left,level-1,fptr);
			printLevel(root->right,level-1,fptr);
		}
	}
}
void level_order(Node *root,int height,FILE *fptr)
{
	for(int i=1;i<=height;i++)
	{
		printLevel(root,i,fptr);
		fprintf(fptr,"\n");
	}
}
int main()
{
	bool done = false;
	unordered_multimap<char,string> mp;
	FILE *fptr = fopen("/home/ritu/toc_ass/toc_ass2/grammar_input.txt","r");
	while(done == false)
	{
		char str[100];
		fscanf(fptr,"%s",str);
		if(strcmp(str,"end") != 0)
		{
			char symbol;
			symbol = str[0];
			int i = 3;
			string non_terminal = "\0";
			while(i < strlen(str))
			{
				if(str[i] != '|')
				{
					while(i < strlen(str) && str[i] != '|')
					{
						non_terminal = non_terminal + getString(str[i]);
						i++;
					}
					mp.emplace(symbol,non_terminal);
				}
				else if(str[i] == '|')
				{
					non_terminal = "\0";
					i++;
				}
			}
		}
		else
			done = true;
	}
	fclose(fptr);
	FILE* fptr1 = fopen("/home/ritu//toc_ass/toc_ass2/output.txt","a");
	fprintf(fptr1,"----------------------------------------------------------------------\n");
	fprintf(fptr1,"INPUT GRAMMAR:\n");
	fprintf(fptr1,"----------------------------------------------------------------------\n");
	for(auto itr = mp.begin();itr != mp.end();itr++)
	{
		fprintf(fptr1,"%c -> %s\n",itr->first,(itr->second).c_str());
	}
	fprintf(fptr,"----------------------------------------------------------------------\n");
	//cout<<"Enter string to check if it belongs to a grammar:";
	char check_string[100];
	FILE *fp = fopen("/home/ritu/toc_ass/toc_ass2/string_input.txt","r");
	fscanf(fp,"%s",check_string);
	fclose(fp);
	fprintf(fptr1,"INPUT STRING: %s",check_string);
	map<int,vector<vector<Node*>>> cyk;
	vector<vector<Node*>> temp;
	for(int i=0;i<strlen(check_string);i++)
	{
		vector<Node*> inner_temp;
		for(auto itr = mp.begin();itr != mp.end();itr++)
		{
			if(itr->second == getString(check_string[i]))
			{
				Node *push_node = (Node*)malloc(sizeof(Node));
				push_node->data = itr->first;
				push_node->left = NULL;
				push_node->right = NULL;
				push_node->leaf = true;
				push_node->terminal = check_string[i];
				inner_temp.push_back(push_node);
			}
		}
		temp.push_back(inner_temp);
		inner_temp.clear();
	}
	cyk[1] = temp;
	temp.clear();
	for(int i = 0;i != cyk[1].size() - 1;i++)
	{
		vector<Node*> left = cyk[1][i];
		vector<Node*> right = cyk[1][i+1];
		vector<Node*> inner_temp;
		for(int j = 0;j<left.size();j++)
		{
			for(int k=0;k<right.size();k++)
			{
				string str_to_push = getString(left[j]->data) + getString(right[k]->data);
				for(auto itr = mp.begin();itr != mp.end();itr++)
				{
					if(itr->second == str_to_push)
					{
						Node *push_node = (Node*)malloc(sizeof(Node));
						push_node->data = itr->first;
						push_node->left = left[j];
						push_node->right = right[k];
						inner_temp.push_back(push_node);
					}
				}
			}
		}
		temp.push_back(inner_temp);
		inner_temp.clear();
	}
	cyk[2] = temp;
	temp.clear();
	int i = 3;
	for(int j = strlen(check_string)-2;j>=1;j--)
	{
		temp.clear();
		for(int k = 0;k<j;k++)
		{
			vector<Node*> inner_temp;
			int j1 = 1;
			int k1 = k;
			int j2 = i-1;
			int k2 = k+1;
			while(j1 < i)
			{
				vector<Node*> left = cyk[j1][k1];
				vector<Node*> right = cyk[j2][k2];
				if(!(left.empty()) && !(right.empty()))
				{
					for(int l = 0;l<left.size();l++)
					{
						for(int m=0;m<right.size();m++)
						{
							string str_to_push = getString(left[l]->data) + getString(right[m]->data);
							for(auto itr = mp.begin();itr != mp.end();itr++)
							{
								if(itr->second == str_to_push)
								{
									Node *push_node = (Node*)malloc(sizeof(Node));
									push_node->data = itr->first;
									push_node->left = left[l];
									push_node->right = right[m];
									inner_temp.push_back(push_node);
								}
							}
						}
					}
				}
				j1++;
				j2--;
				k2++;
			}
			temp.push_back(inner_temp);
			inner_temp.clear();
			
		}
		cyk[i] = temp;
		i++;
	}
	fprintf(fptr1,"\n----------------------------------------------------------------------\n");
	fprintf(fptr1,"CYK table for %s is as follows:\n",check_string);
	fprintf(fptr1,"----------------------------------------------------------------------\n");
	for(int i=strlen(check_string);i>=1;i--)
	{
		//cout<<"Size:"<<cyk[i].size()<<endl;
		for(int j=0;j<cyk[i].size();j++)
		{
			if(!(cyk[i][j].empty()))
			{
				for(auto itr = cyk[i][j].begin();itr != cyk[i][j].end();itr++)
				{	
					fprintf(fptr1,"%c",(*itr)->data);
				}
				fprintf(fptr1,"\t");
			}
			else
				fprintf(fptr1,"empty\t");
		}
		fprintf(fptr1,"\n");
	}
	fprintf(fptr1,"----------------------------------------------------------------------\n");
	int possible_trees = 0;
	vector<Node*> Final = cyk[strlen(check_string)][0];
	fprintf(fptr1,"\n----------------------------------------------------------------------\n");
	fprintf(fptr1,"Possible trees are as follows:\n");
	fprintf(fptr1,"----------------------------------------------------------------------\n");
	for(auto itr = Final.begin();itr != Final.end();itr++)
	{
		if((*itr)->data == 'S')
		{
			int h = height_of(*itr);
			level_order(*itr,h,fptr1);
			fprintf(fptr1,"----------------------------------------------------------------------\n");
			possible_trees++;
		}
	}
	fprintf(fptr1,"\n");
	if(possible_trees == 0)
		fprintf(fptr1,"String %s cannot be generated by the given grammar\n",check_string);
	else if(possible_trees == 1)
		fprintf(fptr1,"Grammar is unambiguous as only 1 parse tree generated by this grammar for string : %s\n",check_string);
	else
	{
		fprintf(fptr1,"Grammar is ambiguous as multiple parse trees for %s are generated by the given grammar\n",check_string);
		fprintf(fptr1,"Number of parse trees generated : %d\n",possible_trees);
	}
	fprintf(fptr1,"______________________________________________________________________\n\n\n");
	fclose(fptr1);
	return 0;
}
