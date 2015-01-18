#ifndef _CDAL_H_
#define _CDAL_H_

//CDAL.h
//Chained Dynamic Array List

//Medhir Bhargava
//COP3530	
//September 23, 2014

#include <iostream>
#include <stdexcept>

namespace cop3530 
{
	template<typename T>
	class CDAL
	{
	public:
		struct Node
		{
			T* data;
			Node* next; 

			Node(Node* t): next(t) 
			{
				data = new T[50];
			}
		};

		typedef Node* link;

		typedef std::size_t size_t;
		typedef T value_type;

		/*
        ITERATOR CLASS DEFINITIONS
        */

        class CDAL_Iter //: public std::<std::forward_iterator_tag, T>
        {
        public:
        	//inheriting from std::iterator<std::forward_iterator_tag, T>
        	//automagically sets up these typedefs...
        	typedef T value_type;
        	typedef std::ptrdiff_t difference_type;
        	typedef T& reference;
        	typedef T* pointer;
        	typedef std::forward_iterator_tag iterator_category;

        	//but not these typedefs
        	typedef CDAL_Iter self_type;
        	typedef CDAL_Iter& self_reference;

        private:
        	Node* current;
        	T* ptr;
        	int index;
        public:
        	explicit CDAL_Iter(Node* start = NULL, int index = 0) : current(start), index(index) {}
        	CDAL_Iter(const CDAL_Iter& src) : current(src.current), index(src.index) {}
        	self_reference operator=(const CDAL_Iter& src)
        	{
        		current = src.current;
        		index = src.index;
        		return *this;
        	}

        	reference operator*() const
        	{
        		return current->data[index%50];
        	}

        	pointer operator->() const
        	{
        		return current;
        	}

        	self_reference operator++()
        	{
        		if(index%50 == 49)
        		{
        			current = current->next;
        			++index;
        		}
        		else
        		{
        			++index;
        		}
        		return *this;
        	}

        	self_reference operator++(int)
        	{
        		CDAL_Iter results = *this;
        		if(index%50 == 49)
        		{
        			current = current->next;
        			++index;
        		}
        		else
        		{
        			++index;
        		}
        		return results;
        	}

        	bool operator==(const CDAL_Iter& rhs) const
        	{
        		return (index == rhs.index);
        	}

        	bool operator!=(const CDAL_Iter& rhs) const
        	{
        		return (index != rhs.index);
        	}
        };

        class CDAL_Const_Iter //: public std::<std::forward_iterator_tag, T>
        {
        public:
        	//inheriting from std::iterator<std::forward_iterator_tag, T>
        	//automagically sets up these typedefs...
        	typedef T value_type;
        	typedef std::ptrdiff_t difference_type;
        	typedef const T& reference;
        	typedef const T* pointer;
        	typedef std::forward_iterator_tag iterator_category;

        	//but not these typedefs
        	typedef CDAL_Const_Iter self_type;
        	typedef CDAL_Const_Iter& self_reference;

        private:
        	const Node* current;
        	int index;
        public:
        	explicit CDAL_Const_Iter(Node* start = NULL, int index = 0) : current(start), index(index) {}
        	CDAL_Const_Iter(const CDAL_Const_Iter& src) : current(src.current), index(src.index) {}
        	self_reference operator=(const CDAL_Const_Iter& src)
        	{
        		current = src.current;
        		index = src.index;
        		return *this;
        	}

        	reference operator*() const
        	{
        		return current->data[index%50];
        	}

        	pointer operator->() const
        	{
        		return current;
        	}

        	self_reference operator++()
        	{
        		if(index%50 == 49)
        		{
        			current = current->next;
        			++index;
        		}
        		else
        		{
        			++index;
        		}
        		return *this;
        	}

        	self_reference operator++(int)
        	{
        		CDAL_Const_Iter results = *this;
        		if(index%50 == 49)
        		{
        			current = current->next;
        			++index;
        		}
        		else
        		{
        			++index;
        		}
        		return results;
        	}

        	bool operator==(const CDAL_Const_Iter& rhs) const
        	{
        		return (index == rhs.index);
        	}

        	bool operator!=(const CDAL_Const_Iter& rhs) const
        	{
        		return (index != rhs.index);
        	}
        };

        typedef CDAL_Iter iterator;
        typedef CDAL_Const_Iter const_iterator;

		//default constructor
		CDAL() : nodeCount(0), numElements(0), tail(-1), head(NULL) {}

		CDAL( const CDAL& rhs )
		{
			nodeCount = rhs.nodeCount; //number of nodes equals rhs
			numElements = rhs.numElements; //number of elements equals rhs
			tail = rhs.tail; //tail is equal to same int


			//create list with same number of nodes as rhs
			link currentNode = rhs.head;
			while(currentNode != NULL)
			{
				push_node();
				currentNode = currentNode->next;
			}

			//add elements to the list
			link rhsNode = rhs.head;
			link thisNode = head;
			int tempCount = nodeCount;
			while(tempCount-- != 0)
			{
				for(int i = 0; i < 50; ++i) 
				{
					thisNode->data[i] = rhsNode->data[i];
				}
				rhsNode = rhsNode->next;
				thisNode = thisNode->next;
			}
		}

		//destructor
		~CDAL()
		{
			clearList();
		}

		//overloaded = operator
		CDAL& operator=( const CDAL& rhs )
		{
			if ( &rhs == this )
				return *this;
			//delete contents of previous list
			clearList();

			nodeCount = rhs.nodeCount; //number of nodes equals rhs
			numElements = rhs.numElements; //number of elements equals rhs
			tail = rhs.tail; //tail is equal to same int


			//create list with same number of nodes as rhs
			link currentNode = rhs.head;
			while(currentNode != NULL)
			{
				push_node();
				currentNode = currentNode->next;
			}

			//add elements to the list
			link rhsNode = rhs.head; //set rhsNode to point to head of right hand side
			link thisNode = head; //set thisNode to point to head of the list being constructed 
			int tempCount = nodeCount; 
			while(tempCount-- != 0) //place elements in rhs node into this node
			{
				for(int i = 0; i < 50; ++i) 
				{
					thisNode->data[i] = rhsNode->data[i];
				}
				rhsNode = rhsNode->next;
				thisNode = thisNode->next;
			}
			return *this;
		}

		//member function definitions
		T replace( const T& element, int position )
		{
			if ( position > tail )
				throw std::out_of_range("Cannot replace an object outside of range!");
			if (position < 0)
				throw std::out_of_range("Cannot replace an object at a negative position");

			int goToNode = position / 50;
			int index = position % 50;

			//navigate to correct node
			link point = head;
			while(goToNode-- != 0)
			{
				point = point->next;
			}

			//swap data, return original data
			T objectData = point->data[index];
			point->data[index] = element;
			return objectData;
		}

		void insert( const T& element, int position )
		{
			if ( position > (tail+1) )
				throw std::out_of_range("Cannot place an object outside of range!");
			if (position < 0)
				throw std::out_of_range("Cannot place an object at a negative position");

			//make sure there is enough space
			checkSize();
			//go to correct node
			int goToNode = position/50;
			link point = head;
			while(goToNode-- != 0)
			{
				point = point->next;
			}

			/* Move all entries down one array index position */
			int currentNodeIndex = position % 50;

			//Store data from array in two separate temp variables
			T tempData = point->data[currentNodeIndex%50];
			T tempData2;
			if(currentNodeIndex%50 != 49)
			{
				tempData2 = point->data[currentNodeIndex+1];
			} 
			else
			{
				tempData2 = point->next->data[0];
			}

			//place element in correct position
			point->data[currentNodeIndex++] = element;

			while(point != NULL)
			{
				while((currentNodeIndex%50) != 49)
				{
					point->data[currentNodeIndex%50] = tempData;
					tempData = tempData2;
					tempData2 = point->data[(currentNodeIndex+1)%50];
					++currentNodeIndex;
				}

				point->data[currentNodeIndex%50] = tempData;
				point = point->next; //move up one node
				tempData = tempData2;
				currentNodeIndex++;
				if(point != NULL)
				{
					tempData2 = point->data[currentNodeIndex%50]; 
				}
			}
			++tail; ++numElements;
		}

		void push_front( const T& element )
		{
			//just use insert method
			insert(element, 0);
		}

		void push_back( const T& element )
		{
			checkSize();

			//navigate to last node
			link point = head;
			while(point->next != NULL)
				point = point->next;

			point->data[(++tail)%50] = element;
			++numElements; 
		}

		T pop_front()
		{
			if(size() == 0)
				throw std::invalid_argument("Cannot remove element from an empty list");

			T objectData = head->data[0];
			remove(0);
			return objectData;
		}

		T pop_back()
		{
			/*
			//navigate to proper node
			int goToNode = tail / 50;
			link point = head;
			while(goToNode-- != 0)
				point = point->next;

			//get item
			int currentNodeIndex = tail % 50;
			T objectData = point->data[currentNodeIndex];
			--tail;

			//return item
			return objectData;
			*/
			if(size() == 0)
				throw std::invalid_argument("Cannot remove element from an empty list");

			return remove(tail);
		}

		T remove( int position )
		{
			if ( position > tail )
				throw std::out_of_range("Cannot remove an object outside of range!");
			if (position < 0)
				throw std::out_of_range("Cannot remove an object at a negative position");

			//nagivate to proper node
			int goToNode = position / 50;
			link point = head;
			while(goToNode-- != 0)
				point = point->next;

			//Store element in temp object and remove from list
			int currentNodeIndex = position % 50;
			T objectData = point->data[currentNodeIndex];

			//pointers to nodes
			link nextPoint = point->next;
			while(point != NULL)
			{
				while(currentNodeIndex < 49) 
				{
					point->data[currentNodeIndex] = point->data[currentNodeIndex+1];
					++currentNodeIndex;
				}
				if(nextPoint != NULL)
				{
					point->data[currentNodeIndex] = nextPoint->data[0];
				}
				currentNodeIndex = 0;
				point = point->next;
				if(nextPoint != NULL)
				{
					nextPoint = nextPoint->next;
				}
			}
			--tail; --numElements;
			return objectData;
		}

		T item_at( int position )
		{
			if ( position > tail )
				throw std::out_of_range("Cannot retrieve an object outside of range!");
			if (position < 0)
				throw std::out_of_range("Cannot retrieve an object at a negative position");

			//navigate to correct node
			int goToNode = position / 50;
			link point = head;
			while(goToNode-- != 0)
				point = point->next;

			//retrieve element;
			int currentNodeIndex = position % 50;
			return point->data[currentNodeIndex];
		}

		T& operator[](int i) 
        {
        	if ( i > tail )
				throw std::out_of_range("Cannot retrieve an object outside of range!");
			if (i < 0)
				throw std::out_of_range("Cannot retrieve an object at a negative position");

            //navigate to correct node
			int goToNode = i / 50;
			link point = head;
			while(goToNode-- != 0)
				point = point->next;

			//retrieve element;
			int currentNodeIndex = i % 50;
			return point->data[currentNodeIndex];
        }

        T const& operator[](int i) const
        {
        	if ( i > tail )
				throw std::out_of_range("Cannot retrieve an object outside of range!");
			if ( i < 0)
				throw std::out_of_range("Cannot retrieve an object at a negative position");

            //navigate to correct node
			int goToNode = i / 50;
			link point = head;
			while(goToNode-- != 0)
				point = point->next;

			//retrieve element;
			int currentNodeIndex = i % 50;
			return point->data[currentNodeIndex];
        }

		bool is_empty() const
		{
			if(tail == -1)
				return true;
			return false;
		}

		size_t size() const 
		{
			return numElements;
		}

		void clear()
		{
			clearList();
		}

		bool contains( const T& element, bool equals( const T& a, const T& b )) const
		{
			int currentElement = 0;
			link point = head;
			while(point != NULL)
			{
				for(int i = 0; i < 50; ++i)
				{
					if(equals(point->data[currentElement%50], element))
						return true;
					++currentElement;
				}
				point = point->next;
			}
			return false;
		}

		std::ostream& print( std::ostream& out ) const 
		{
			if(head == NULL)
			{
				out << "<empty list>" << std::endl;
				return out;
			}
			else 
			{
				out << "[";
				int currentElement = 0;
				link point = head;
				while(point != NULL)
				{
					for(int i = 0; i < 50; ++i)
					{
							if(currentElement < tail)
							{
								out << point->data[(currentElement++)%50] << ", ";	
							}
							else if (currentElement == tail)
							{
								out << point->data[(currentElement++)%50];
							}
					}
					point = point->next;
				}
				out << "]";
				return out;
			}
		}

		iterator begin() { return CDAL_Iter( head, 0 ); }
    	iterator end() { return CDAL_Iter( head , tail+1 ); }
     
     	const_iterator begin() const { return CDAL_Const_Iter( head, 0 ); }
     	const_iterator end() const { return CDAL_Const_Iter( head, tail+1 ); } 

	private:
		int numElements, nodeCount, tail;
		void checkSize()
		{
			//if CDAL contains no elements
			if(head == NULL)
			{
				head = new Node(NULL); //create a new node
				++nodeCount; //increment nodeCount
			}

			if((tail%50) == 49)
			{
				link lastNode = new Node(NULL);

				//traverse to last node currently existing
				link point = head;
				while(point->next != NULL)
				{
					point = point->next;
				}

				//add lastNode to list
				point->next = lastNode;
				++nodeCount;
			}
		} 

		link head;

		void clearList()
		{
			for(link t = head; t != NULL; head = t)
			{
				delete [] t->data; //delete array
				t = t->next;
				delete head; //delete node that originally stored the array
			}
			numElements = 0; nodeCount = 0; tail = -1;
			head = NULL;
		}

		void push_node()
		{
			if(is_empty())
			{
				head = new Node(NULL);
			}
			link frontNode = new Node(head);
			head = frontNode;
		}
	};
}

#endif