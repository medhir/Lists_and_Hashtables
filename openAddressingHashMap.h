#include <stdlib.h>
#include <time.h>

template <typename KEY, typename VALUE, 
			typename HASH_FUNC, HASH_FUNC hash, 
			typename PROBE_FUNC, PROBE_FUNC probe,
			bool op_equals = true, typename EQ_FUNC = bool, EQ_FUNC eq_test = false>
class hash_map_oa_1
{
private:
	VALUE nullValue; //represents sentinel values to mark unused slot
	KEY nullKey; 

	struct Node //object that will hold key/value pairs
	{	
		KEY key; 
		VALUE value;
		Node() {}
		Node(KEY k, VALUE v) : key(k), value(v) {} //node constructor
	};
	Node* hashTable; //pointer to array of Nodes (hash Table)
	std::size_t N, M; //used slots, capacity
public:
	hash_map_oa_1()
	{
		N = 0; M = 100;
		hashTable = new Node[M];
		for (int i = 0; i < M; ++i) //set all slot values to NULL
		{
			hashTable[i].key = nullKey;
			hashTable[i].value = nullValue;
		}
	}

	hash_map_oa_1(std::size_t maxN)
	{
		N = 0; M = 2*maxN;
		hashTable = new Node[M];
		for (int i = 0; i < M; ++i) //set all slot values to NULL
		{
			hashTable[i].key = nullKey;
			hashTable[i].value = nullValue;
		}
	}

	int insert(KEY k, VALUE v)
	{
		if(size() != capacity())
		{
			//initially, hash the key value
			int h = hash(k, M);
			int i = 0;
			while(hashTable[h].value != nullValue)
			{
				++i;
				h = (h + probe(i)) % M;
			}
			hashTable[h].key = k;
			hashTable[h].value = v;
			++N;
			return i;
		}
		else
			return 0;
	}

	int remove(KEY k, VALUE& v)
	{
		//initially, hash the key value
		int h = hash(k, M);
		int i = 0;

		while(hashTable[h].value != nullValue)
		{
			if(hashTable[h].key != k)
			{
				++i;
				h = (h + probe(i)) % M;
			}
			else if (hashTable[h].key == k)
			{
				v = hashTable[h].value;
				hashTable[h].key = nullKey;
				hashTable[h].value = nullValue;
				h = (h+1)%M;
				--N;
				while(hashTable[h].value != nullValue)
				{
					KEY tempKey = hashTable[h].key; 
					VALUE tempVal = hashTable[h].value;
					hashTable[h].key = nullKey;
					hashTable[h].value = nullValue;
					--N;
					h = (h+1)%M;
					insert(tempKey, tempVal);
				}
				return i;
			}
		}
		v = nullValue;
		return -1*i;
	}

	int search(KEY key, VALUE& value)
	{
		if(size() == capacity())
		{
			for(int i = 0; i < M; ++i)
			{
				if(hashTable[i].key == key)
				{
					value = hashTable[i].value;
					return i;
				}
			}
			value = nullValue;
			return -1*M;
		}
		else
		{
			//initially, hash the key value
			int h = hash(key, M);
			int i = 0;

			while(hashTable[h].value != nullValue)
			{
				if(hashTable[h].key != key)
				{
					++i;
					h = (h + probe(i)) % M;
				}
				else if (hashTable[h].key == key)
				{
					value = hashTable[h].value;
					return i;
				}
			}
			value = nullValue;
			return -1*i;
		}
	}

	void clear()
	{
		for(int i = 0; i < M; ++i)
		{
			hashTable[i].key = nullKey;
			hashTable[i].value = nullValue;
		}
		N = 0;
	}

	bool isEmpty() const
	{
		return N == 0;
	}

	std::size_t capacity()
	{
		return M;
	}

	std::size_t size()
	{
		return N;
	}

	double load()
	{
		return (double) N / (double) M;
	}

	void print(std::ostream& out)
	{
		out << "[ ";
			for(int i = 0; i < M; ++i)
			{
				if(hashTable[i].value == nullValue)
					out << "- ";
				else
					out << hashTable[i].value << " ";
			}
			out << "]";
	}

	void cluster_distribution(std::ostream& out)
	{
		if(size() == capacity())
			out << "Size " << M << ": 1 Cluster\n";
		else
		{
			int i = 0; 
			while(hashTable[i].value != nullValue)
				++i;

			//store number of clusters present in an array
			int clusters[M];
			for(int j = 0; j < M; ++j)
				clusters[j] = 0;

			int clusterCount = 0; 

			for(int j = i; j <= (M + i); ++j)
			{
				if(hashTable[j%M].value != nullValue)
					++clusterCount;
				else 
				{
					++clusters[clusterCount]; //add the cluster to the clusters[] array
					clusterCount = 0; //reset the int counting number of elements in cluster
				}
			}

			for(int j = 1; j < M; ++j)
			{
				if(clusters[j] != 0)
				{
					out << "Size " << j << ": " << clusters[j] << " " << "clusters\n";
				}
			}
		}
	}

	KEY remove_random()
	{
		srand(time(NULL));
		int removeRand = rand() % M + 1;
		
		//find the R-th occupied spot
		int rIndex = 0;
		while(removeRand != 0)
		{
			if(hashTable[rIndex].value != nullValue)
				--removeRand;
			rIndex = (rIndex + 1) % M;
		}
		KEY tempKey = hashTable[rIndex].key;
		VALUE tempVal;
		remove(tempKey, tempVal);
		return tempKey;
	}
};

template <typename KEY, typename VALUE, 
			typename HASH_FUNC, HASH_FUNC hash, 
			typename HASH_FUNC2, HASH_FUNC2 hash2,
			bool op_equals = true, typename EQ_FUNC = bool, EQ_FUNC eq_test = false>
class hash_map_oa_2
{
private:
	VALUE nullValue; //represents sentinel values to mark unused slot
	KEY nullKey; 

	struct Node //object that will hold key/value pairs
	{	
		KEY key; 
		VALUE value;
		Node() {}
		Node(KEY k, VALUE v) : key(k), value(v) {} //node constructor
	};
	Node* hashTable; //pointer to array of Nodes (hash Table)
	std::size_t N, M; //used slots, capacity
public:
	hash_map_oa_2()
	{
		N = 0; M = 100;
		hashTable = new Node[M];
		for (int i = 0; i < M; ++i) //set all slot values to NULL
		{
			hashTable[i].key = nullKey;
			hashTable[i].value = nullValue;
		}
	}

	hash_map_oa_2(std::size_t maxN)
	{
		N = 0; M = 2*maxN;
		hashTable = new Node[M];
		for (int i = 0; i < M; ++i) //set all slot values to NULL
		{
			hashTable[i].key = nullKey;
			hashTable[i].value = nullValue;
		}
	}

	int insert(KEY k, VALUE v)
	{
		if(size() != capacity())
		{
			//initially, hash the key value
			int h = hash(k, M);
			int i = 0;
			while(hashTable[h].value != nullValue)
			{
				++i;
				h = (h + i*hash2(k, v)) % M;
			}
			hashTable[h].key = k;
			hashTable[h].value = v;
			++N;
			return i;
		}
		else
			return 0;
	}

	int remove(KEY k, VALUE& v)
	{
		//initially, hash the key value
		int h = hash(k, M);
		int i = 0;

		while(hashTable[h].value != nullValue)
		{
			if(hashTable[h].key != k)
			{
				++i;
				h = (h + i*hash2(k, v)) % M;
			}
			else if (hashTable[h].key == k)
			{
				v = hashTable[h].value;
				hashTable[h].key = nullKey;
				hashTable[h].value = nullValue;
				h = (h+1)%M;
				--N;
				while(hashTable[h].value != nullValue)
				{
					KEY tempKey = hashTable[h].key; 
					VALUE tempVal = hashTable[h].value;
					hashTable[h].key = nullKey;
					hashTable[h].value = nullValue;
					--N;
					h = (h+1)%M;
					insert(tempKey, tempVal);
				}
				return i;
			}
		}
		v = nullValue;
		return -1*i;
	}

	int search(KEY k, VALUE& v)
	{
		if(size() == capacity())
		{
			for(int i = 0; i < M; ++i)
			{
				if(hashTable[i].key == k)
				{
					v = hashTable[i].value;
					return i;
				}
			}
			v = nullValue;
			return -1*M;
		}
		else
		{
			//initially, hash the key value
			int h = hash(k, M);
			int i = 0;

			while(hashTable[h].value != nullValue)
			{
				if(hashTable[h].key != k)
				{
					++i;
					h = (h + i*hash2(k, v)) % M;
				}
				else if (hashTable[h].key == k)
				{
					v = hashTable[h].value;
					return i;
				}
			}
			v = nullValue;
			return -1*i;
		}
	}

	void clear()
	{
		for(int i = 0; i < M; ++i)
		{
			hashTable[i].key = nullKey;
			hashTable[i].value = nullValue;
		}
		N = 0;
	}

	bool isEmpty() const
	{
		return N == 0;
	}

	std::size_t capacity()
	{
		return M;
	}

	std::size_t size()
	{
		return N;
	}

	double load()
	{
		return (double) N / (double) M;
	}

	void print(std::ostream& out)
	{
		out << "[ ";
			for(int i = 0; i < M; ++i)
			{
				if(hashTable[i].value == nullValue)
					out << "- ";
				else
					out << hashTable[i].value << " ";
			}
			out << "]";
	}

	void cluster_distribution(std::ostream& out)
	{
		if(size() == capacity())
			out << "Size " << M << ": 1 Cluster\n";
		else
		{
			int i = 0; 
			while(hashTable[i].value != nullValue)
				++i;

			//store number of clusters present in an array
			int clusters[M];
			for(int j = 0; j < M; ++j)
				clusters[j] = 0;

			int clusterCount = 0; 

			for(int j = i; j <= (M + i); ++j)
			{
				if(hashTable[j%M].value != nullValue)
					++clusterCount;
				else 
				{
					++clusters[clusterCount]; //add the cluster to the clusters[] array
					clusterCount = 0; //reset the int counting number of elements in cluster
				}
			}

			for(int j = 1; j < M; ++j)
			{
				if(clusters[j] != 0)
				{
					out << "Size " << j << ": " << clusters[j] << " " << "clusters\n";
				}
			}
		}
	}

	KEY remove_random()
	{
		srand(time(NULL));
		int removeRand = rand() % M + 1;
		
		//find the R-th occupied spot
		int rIndex = 0;
		while(removeRand != 0)
		{
			if(hashTable[rIndex].value != nullValue)
				--removeRand;
			rIndex = (rIndex + 1) % M;
		}
		KEY tempKey = hashTable[rIndex].key;
		VALUE tempVal;
		remove(tempKey, tempVal);
		return tempKey;
	}
};