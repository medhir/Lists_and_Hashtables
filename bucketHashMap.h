#include <stdlib.h>
#include <time.h>

namespace cop3530 {
	template <typename KEY, typename VALUE, 
	typename HASH_FUNC, HASH_FUNC hash,
	bool op_equals = true, typename EQ_FUNC = bool, EQ_FUNC eq_test = false>
	class BHash
	{
	private:
		typedef KEY Key;
		typedef VALUE Value; 
		Value nullValue;
		int N, M;

		struct Node
		{
			Key key;
			Value value;
			Node *next;

			Node(Key k, Value v, Node* n) : key(k), value(v), next(n) {}
		};

		typedef Node *link;

		link* heads;

		//recursively defined search function
		Value search(link t, Key v)
		{
			if (t == 0) 
				return nullValue;
			if (t->key == v) 
				return t->value;
			else
				return search(t->next, v);
		}

	public:
		BHash(int maxN)
		{
			N = 0; M = maxN/5;
			heads = new link[M];
			for(int i = 0; i < M; ++i)
			{
				heads[i] = NULL;
			}
		}
		int insert(Key key, Value value)
		{
			int i = hash(key, M);
			link newNode = new Node(key, value, heads[i]);
			heads[i] = newNode;
			++N;
			return 1;
		}
		int remove(Key key, Value& value)
		{
			int i = hash(key, M);
			//establish pointers to find key value, then remove selected item
			link prev = NULL;
			link curr = heads[i];

			while(curr->key != key && curr != NULL)
			{
				prev = curr;
				curr = curr->next;
			}

			if(curr == NULL)
				return -1;

			if(prev != NULL)
			{
				value = curr->value;
				prev->next = curr->next;
				delete curr;
				--N;
				return 1;
			}
			else
			{
				value = curr->value;
				heads[i] = curr->next;
				delete curr; 
				--N;
				return 1;
			}
		}

		int search(Key key, Value& value)
		{
			int i = hash(key, M);
			link curr = heads[i];
			
			Value valStore = search(curr, key);

			if(valStore == nullValue)
				return -1;
			else
			{
				value = valStore;
				return 1;
			}
		}
		void clear()
		{
			//traverse array of links
			for(int i = 0; i < M; ++i)
			{
				//start to clear list if the slot's pointer isn't null
				while(heads[i] != NULL)
				{
					link temp = heads[i];
					heads[i] = heads[i]->next;
					delete temp;
				}
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
			double slotCount;
			for(int i = 0; i < M; ++i)
			{
				if(heads[i] != NULL)
					++slotCount;
			}

			return slotCount / (double) M;
		}
		void print(std::ostream& out)
		{
			out << "[ ";
			for(int i = 0; i < M; ++i)
			{
				link curr = heads[i];
				if( curr == NULL )
					out << "- ";
				while(curr != NULL)
				{
					out << curr->value << " ";
					curr = curr->next;
				}
			}
			out << "]";
		}

		void cluster_distribution(std::ostream& out)
		{
			if(size() == capacity())
				out << "Size " << M << ": 1 Cluster\n";
			if(size() > capacity())
				out << "Size " << size() << ": 1 Cluster\n";
			else
			{
				int i = 0; 
				while(heads[i] != NULL)
					++i;

				//store number of clusters present in an array
				int clusters[M];
				for(int j = 0; j < M; ++j)
					clusters[j] = 0;

				int clusterCount = 0; 

				for(int j = i; j <= (M + i); ++j)
				{
					if(heads[j%M] != NULL)
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
				if(heads[rIndex] != NULL)
					--removeRand;
				rIndex = (rIndex + 1) % M;
			}
			KEY tempKey = heads[rIndex]->key;
			VALUE tempVal;
			remove(tempKey, tempVal);
			return tempKey;
		}
	};
}