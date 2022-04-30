/*
 *      Author: kompalli
 */
#include "IPAddressAnalyze.h"
#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;
long tSize = 50000000;	 //size of the the hash table
long max_req = 0; //the max number of request stored here
string str13;

struct node //defining the node class(struct)
{
	long ipadd;	  // The ipAddress as converted to long
	int req;
	long IPconverted;	  //The number of request
	node *p_node; // prevoous node ponter
	node *n_node; //next node pointer
};

//Start of heap class // source: https://www.geeksforgeeks.org/binary-heap/

class MaxHeap
{
private:
	node *Heep;			  //node creation for the heap class
	int h_size, max_size; //the maximum size and the actuall siwze variables
public:
	MaxHeap(long capacity) // MAx heap functiom
	{
		h_size = 0;
		max_size = capacity;
		Heep = new node[max_size];
	}

	bool heapFull() //function to check if the heap is full or not returns true or false
	{
		// throw std::range_error("No more space int the heap");
		if (h_size == max_size)
			return true;
		return false;
	}

	void insert(long address, int req) //function to insert into the heap
	{
		if (heapFull()) //error message when the heap is full
		{
			throw std::range_error("No more space int the heap");
		}

		Heep[h_size].req = req;
		Heep[h_size].ipadd = address;
		int ind_child = h_size;
		int ind_parent = (ind_child - 1) / 2;

		//sapping parent for child while child is greater than parent until root
		while (ind_parent >= 0 && Heep[ind_parent].req < Heep[ind_child].req)
		{
			node temp = Heep[ind_parent];
			Heep[ind_parent] = Heep[ind_child];
			Heep[ind_child] = temp;
			ind_child = ind_parent;
			ind_parent = (ind_parent - 1) / 2;
		}

		h_size++;
	}
	int pcheck()
	{
		return Heep[0].req;
	}

	void To_File(int cc)
	{
		string str11, str12;
		struct in_addr addr;
		node arrange[10];
		int dup_size = 0;
		arrange[dup_size] = Heep[0];
		arrange[dup_size].IPconverted = hashedNumber(Heep[0].ipadd);
		pop();
		while(arrange[0].req == Heep[0].req && !is_Empty())
		{
			dup_size++;
			arrange[dup_size] = Heep[0];
			arrange[dup_size].IPconverted = hashedNumber(Heep[0].ipadd);
			pop();
		}

	
		for(int i = 0; i <= dup_size; i++)
		{
			for(int j= 0; j <=dup_size; j++)
			{
				if(arrange[i].IPconverted < arrange[j].IPconverted)
				{
					
					node temp = arrange[i];
					arrange[i] = arrange[j];
					
					arrange[j] = temp;
				}
			}
		}
		hashedNumber(arrange[0].ipadd);
		for(int k = 0; k<=dup_size; k++)
		{
			addr.s_addr = arrange[k].ipadd;
			char *dot_ipv = inet_ntoa(addr);

			str11 = to_string(cc);
			str12 = to_string(arrange[k].req);
			str13 = str13 + str11 + ", " + dot_ipv + ", " + str12 + "\n";//concatenating the results to string that will then be printed in outputfile

		} 
		
	}

	long hashedNumber(long IP)
	{
		struct in_addr addr;
		addr.s_addr = IP;
		char *dot_ipv = inet_ntoa(addr);

		
		string numb1; 
		string numb2; 
		string numb3; 
		string numb4; 

		std::string s = dot_ipv;
		char temp_char[20];
		strcpy(temp_char, s.c_str());
		char *token = strtok(temp_char, ".");
		numb1 = token;
		token = strtok(NULL, ".");
		numb2 = token;
		token = strtok(NULL, ".");
		numb3 = token;
		token = strtok(NULL, ".");
		numb4 = token;

		string smallIP[] = {numb1, numb2, numb3, numb4};

		for(int k = 0; k < 4; k++)
		{
			while(smallIP[k].length() < 3)
			{
				smallIP[k]  = "0" + smallIP[k];
			}
		}

		string IPnum = smallIP[0] + smallIP[1] + smallIP[2] + smallIP[3];

		return stol(IPnum);
		
	}

	node pop() //pop the last item from the heap after printing
	{
		node max = Heep[0];
		Heep[0] = Heep[h_size - 1];
		h_size--;
		maxHeapify(0);
		return max;
	}
	//
	void maxHeapify(int index) //maxheapify function for building the max heap
	{

		int l_child = 2 * index + 1; //left child for that node
		int r_child = 2 * index + 2; //right child for the node

		int ind_max = index;
		if (l_child <= h_size && Heep[l_child].req > Heep[ind_max].req)
		{

			ind_max = l_child;
		}
		if (r_child <= h_size && Heep[r_child].req > Heep[ind_max].req)
		{

			ind_max = r_child;
		}

		if (ind_max != index) //swapping the max index with the index and calling the fuction on itself to rearrenge
		{
			node temp = Heep[ind_max];
			Heep[ind_max] = Heep[index];
			Heep[index] = temp;

			maxHeapify(ind_max);
		}
	}

	bool is_Empty() //to check if the linked list is empty
	{
		if (h_size == 0)
		{
			return true;
		}
		return false;
	}

	void print(int topn1) //function to travers the list and print then pop for each node
	{
		int cnt = 0;
		int nreq1, nreq2;
		while (!is_Empty() && cnt < topn1)
		{
			To_File(cnt + 1);
			// nreq1 = pcheck(); //getting the request number before the pop
			// pop();
			// nreq2 = pcheck(); //getting the request number after the pop
			// if (nreq1 == nreq2)
			// { //nullify the incrementation if the number of request befor is equal to the number of request after
			// 	cnt = cnt - 1;
			// }
			cnt++;
		}
	}
};

//end heap class

// source oflinked list implementation :https://www.studytonight.com/data-structures/doubly-linked-list
//begin linked list class

class double_LL
{


public:
	node *front; // pointer to the first node of the list
	node *end;	 // pointer to the last node of the list
	// MaxHeap maxHeap1 = MaxHeap(100000000);
	double_LL()
	{
		front = NULL;
		end = NULL;
	}
	void add_end(long d, int r)
	{
		// create new node
		node *temp;
		temp = new node();
		temp->ipadd = d;
		temp->req = r;
		temp->p_node = end;
		temp->n_node = NULL;

		if (r > max_req)
		{
			max_req = r;
		}
		// if list is empty
		if (end == NULL)
			front = temp;
		else
			end->n_node = temp;
		end = temp;
	}
	int check(long ch) //checking the list to see if the address is already there
	{
		node *traverse;
		traverse = front;
		while (traverse != NULL) //traversing the list and checking if address is in it
		{
			if (traverse->ipadd == ch)
			{
				return 0;
			}
			traverse = traverse->n_node;
		}
		return 1;
	}
	void increm(long key, int re) //increments the req for addresses that have been seen before
	{
		node *traverse;
		traverse = front;
		while (traverse != NULL)
		{

			// cout<<traverse->data<<endl;
			if (traverse->ipadd == key)
			{
				traverse->req = traverse->req + re;

				if (traverse->req > max_req)
				{
					max_req = traverse->req;
				}
			}
			traverse = traverse->n_node;
		}
	}
	void forward_search(int maxim) //seasrching for the seen address
	{
		node *traverse;
		traverse = front;
		while (traverse != NULL)
		{
			struct in_addr addr;
			addr.s_addr = traverse->ipadd;
			char *dot_ipv = inet_ntoa(addr);
			if (maxim == traverse->req)
			{
				std::cout << dot_ipv << " , " << traverse->req << " => ";
			}
			traverse = traverse->n_node;
		}
	}
	void forwardHeaping(long hSize, int topn, MaxHeap maxHeap112) //going through the list and inserting to the tree
	{
		// node *traverse;
		// traverse = front;
		// while (traverse != NULL)
		// {
		// 	cout<<traverse->ipadd<<" , "<< traverse->req<<endl;
		// 	maxHeap112.insert(traverse->ipadd, traverse->req); //inserting to the heap tree using function

		// 	traverse = traverse->n_node;
		// }
		// maxHeap1.print(topn);
	}
};
//end linked list imp

//start of int code //source https://www.educative.io/edpresso/how-to-implement-a-hash-table-in-cpp
class HashTable
{ //begining of defining the hash table function with finctions: constructor, get hash,insert and remove element
private:
	double_LL *table;
	long total_elements;
	// Hash function to calculate hash for an ip and return the key
	long hash_it(long kk)
	{
		long key =kk % total_elements;//calculating the key by doing the simple modulus with the number of spots in the hash table
		return key;
	}

public:
	// Constructor to create a hash table with 'n' indices:
	HashTable(long numbs)
	{
		total_elements = numbs;
		table = new double_LL[total_elements];
	}

	// Insert data in the hash table:
	void insert_hashTable(long key, int rek)//inserting into the hash table
	{
		if (table[hash_it(key)].check(key) == 0)//if the address already exists incremenyt
		{
			//increment
			table[hash_it(key)].increm(key, rek);
		}
		else//if the address sdoes not already exist add to table using the hashed key
		{
			table[hash_it(key)].add_end(key, rek);
		}
	}


	void GetTopN(int top, MaxHeap maxHeap113)
	{
		// Traversing each index of the hashtable 
		for (int i = 0; i < total_elements; i++)
		{
			// table[i].forwardHeaping(10000000, top, maxHeap113);
				node *traverse;
		traverse = table[i].front;
		while (traverse != NULL)
		{
			// cout<<traverse->ipadd<<" , "<< traverse->req<<endl;
			maxHeap113.insert(traverse->ipadd, traverse->req); //inserting to the heap tree using function
			
			traverse = traverse->n_node;
		}
		
		}
		maxHeap113.print(top);
	}
};
//end of def hastable class

HashTable H_table1(tSize); //create hash table of set size

//end int code

int IPAddressAnalyzer::readNextItemFromFile(FILE *inputFileStream)
{

	if (!inputFileStream)
	{
		std::string message("Cannot open input file for reading");
		throw std::invalid_argument(message.c_str());
	}
	if (!feof(inputFileStream))
	{

		//Variable declaration
		char inpt2[255];

		fgets(inpt2, 255, inputFileStream); // getting the line of char array
		if (inpt2[0] == '\n')
		{
			return 0;
		}

		// begin extracting the ip addres and the number of requests
		char *ip;
		int no_requests;
		std::string s = inpt2;
		char temp_char[20];
		strcpy(temp_char, s.c_str());
		char *token = strtok(temp_char, ",");
		ip = token;
		token = strtok(NULL, ",");
		no_requests = atoi(token);
		// std::cout << "Ip address :  " << ip << "  number of requests :  " << no_requests << std::endl;
		//end extracting the ip addres and the number of requests

		//start convert to long and insert in table//http://www.cplusplus.com/forum/general/9403/
		long long_address = inet_addr(ip);
		H_table1.insert_hashTable(long_address, no_requests); //insert the long
		//end convert

		//end int

		return 0;
	}

	return 0;
}
MaxHeap maxHeap11 = MaxHeap(100000000);
void IPAddressAnalyzer::getMostFrequentIPAddress(char *inputFilePath, char *outputFilePath, int ns)
{

	FILE *inFileStream = fopen(inputFilePath, "r");
	if (!inFileStream)
	{
		char message[1024];
		sprintf(message, "Cannot open input file for reading: %s", inputFilePath);
		throw std::ios_base::failure(message);
		std::cout << "khijbkhjbkjbljhdfbf";
	}

	FILE *outFileStream = fopen(outputFilePath, "w");

	if (!outFileStream)
	{
		char message[1024];
		sprintf(message, "Cannot open output file for writing: %s", outputFilePath);
		throw std::ios_base::failure(message);
	}

	LogManager::writePrintfToLog(LogManager::Level::Status,
								 "IPAddressAnalyzer::getMostFrequentIPAddress",
								 "Starting to process file %s", inputFilePath);

	while (!feof(inFileStream))
	{
		readNextItemFromFile(inFileStream);
	}
	// std::cout << "the max" << max_req << std::endl;
	int maxi = max_req;
	H_table1.GetTopN(ns,maxHeap11);//function that responsible for heaping

	// maxHeap11.print(ns);

	fprintf(outFileStream, "%s", str13.c_str());
	// std::cout<<str13;

	fflush(outFileStream);
	fclose(outFileStream);
	fclose(inFileStream);
}