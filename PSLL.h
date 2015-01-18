#ifndef _PSLL_H_
#define _PSLL_H_

/*
PSLL.h

Pool-using Singly-linked list
Medhir Bhargava
September 23, 2014
for COP 3530
*/

#include <iostream>
#include <stdexcept>

namespace cop3530
{
	template <typename T>
	class PSLL
	{
	public:

		typedef std::size_t size_t;
        typedef T value_type;

		struct Node
		{
			T data;
			Node *next;

			Node(T value, Node *t): data(value), next(t) {}
		}; //end struct Node

		typedef Node *link;

		/*
        ITERATOR CLASS DEFINITIONS
        */

        class PSLL_Iter //: public std::<std::forward_iterator_tag, T>
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
            typedef PSLL_Iter self_type;
            typedef PSLL_Iter& self_reference;

        private: 
            Node* current;

        public:
            explicit PSLL_Iter(Node* start = NULL) : current(start) {}
            PSLL_Iter(const PSLL_Iter& src) : current(src.current) {}
            self_reference operator=(const PSLL_Iter& src)
            {
                current = src.current;
                return *this;
            }

            reference operator*() const
            {
                return current->data;
            }

            pointer operator->() const
            {
                return current;
            }

            self_reference operator++()
            {
                current = current->next;
                return *this;
            }//preincrement

            self_reference operator++(int)
            {
                PSLL_Iter results(*this);
                current = current->next;
                return results;
            } //post increment

            bool operator==(const PSLL_Iter& rhs) const
            {
                return (current == rhs.current);
            }

            bool operator!=(const PSLL_Iter& rhs) const
            {
                return (current != rhs.current);
            }
        }; //end PSLL_Iter

        class PSLL_Const_Iter //: public std::<std::forward_iterator_tag, T>
        {
        public:
            //inheriting from std::iterator<std::forward_iterator_tag, T>
            //automagically sets up these typedefs...
            typedef T value_type;
            typedef std::ptrdiff_t difference_type;
            typedef const T& reference;
            typedef const T* pointer;
            typedef std::forward_iterator_tag iterator_category;

            //but not these typedefs..
            typedef PSLL_Const_Iter self_type;
            typedef PSLL_Const_Iter& self_reference;

        private:
            const Node* current;

        public:
            explicit PSLL_Const_Iter(const Node* start = NULL) : current( start ) {}
            PSLL_Const_Iter(const PSLL_Iter& src) : current( src.current ) {}
            PSLL_Const_Iter(const PSLL_Const_Iter& src) : current( src.current ) {}
            
            self_reference operator=( const PSLL_Iter& src ) 
            {
                current = src.current;
                return *this;
            }

            self_reference operator=( const PSLL_Const_Iter& src ) 
            {
                current = src.current;
                return *this;
            }

            reference operator*() const 
            {
                return current->data;
            }

            pointer operator->() const 
            {
                return current; 
            }

            self_reference operator++()
            {
                current = current->next;
                return *this;
            } //preincrement

            self_type operator++(int) 
            {
                PSLL_Const_Iter results(*this);
                current = current->next;
                return results;
            } //postincrement

            bool operator==( const PSLL_Const_Iter& rhs ) const 
            {
                return (current == rhs.current);
            }
            bool operator!=( const PSLL_Const_Iter& rhs ) const 
            {
                return (current != rhs.current);
            }
        }; //end PSLL_Const_Iter

        typedef PSLL_Iter iterator;
        typedef PSLL_Const_Iter const_iterator;  

		//--------------------------------------------------
		PSLL(): head(NULL), tail(NULL), freeList(NULL), nodeCount(0), freeCount(0) {} //default constructor

		//--------------------------------------------------
		PSLL( const PSLL& rhs ): head(NULL), tail(NULL), freeList(NULL), nodeCount(0), freeCount(0) 
		{
			link currentNode = rhs.head;
			while(currentNode != NULL)
			{
				push_back(currentNode->data); //Add data from src's node
				currentNode = currentNode->next;
			}

			currentNode = rhs.freeList; //switch current to point to pool
			while(currentNode != NULL)
			{
				addNode(currentNode);
				currentNode = currentNode->next;
			}
		}

		//--------------------------------------------------

		~PSLL() {
			for(link t = head; t != 0; head = t) //delete list
			{
				t = head->next;
				delete head;
			}
			head = NULL; tail = NULL;

			for(link t = freeList; t != 0; freeList = t) //delete pool
			{
				t = freeList->next;
				delete freeList;
			}
		}

		//--------------------------------------------------

		PSLL& operator=( const PSLL& rhs ) 
		{
			if( &rhs == this )
				return *this;
			~PSLL();
			link currentNode = rhs.head;
			while(currentNode != NULL)
			{
				push_front(currentNode->data);
				currentNode = currentNode->next;
			}
			currentNode = rhs.freeList;
			while(currentNode != NULL)
			{
				addNode(currentNode);
				currentNode = currentNode->next;
			}
			return *this;
		}

		//member function definitions
		T replace( const T& element, int position ) 
		{
			if ( position >= nodeCount )
				throw std::out_of_range("Cannot replace an object outside of range!");
			if (position < 0)
				throw std::out_of_range("Cannot replace an object at a negative position");

			link point = head;
            T temp;
            int count = position;
            while(point != NULL && count-- != 0)
            {
                point = point->next;
            }
            temp = point->data;
            point->data = element;
            return temp;
		}

		//--------------------------------------------------

		void insert( const T& element, int position) 
		{	
			if ( position > nodeCount )
				throw std::out_of_range("Cannot place an object outside of range!");
			if (position < 0)
				throw std::out_of_range("Cannot place an object at a negative position");

			link new_el;
			if(freeList == NULL)
				new_el = new Node(element, NULL);
			else
			{
				new_el = getNode();
				new_el->data = element; new_el->next = NULL;
			}
			
			link point = head, prev = NULL;
			int count = position;

			if(count == 0 && is_empty())
			{
				head = new_el;
				tail = new_el;
				++nodeCount;
			}
			else if (count == 0)
			{
				new_el->next = head;
				head = new_el;
				++nodeCount;
			}
			else
			{
				while(point != NULL && count-- != 0)
				{
					prev = point;
					point = point->next;
				}

				prev->next = new_el;
				new_el->next = point;
				++nodeCount;
			}
		}

		//--------------------------------------------------

		void push_front( const T& element )
		{
			link new_el = NULL;
			if(freeList == 0)
			{
				new_el = new Node(element, NULL);
			}
			else
			{
				new_el = getNode();
			}
			if(is_empty())
            {
   
                head = new_el;
                tail = new_el;
                ++nodeCount;
            }
            else
            {
                new_el->next = head;
                head = new_el; //head now points to new_el;
                ++nodeCount;
            }
		}

		//--------------------------------------------------

		void push_back( const T& element ) 
		{
			link new_el = NULL;
			if(freeList == 0)
			{
				new_el = new Node(element, NULL);
			}
			else
			{
				new_el = getNode();
			}
			if(is_empty())
            {
                head = new_el;
                tail = new_el;
                ++nodeCount;
            }
            else
            {
                link point = tail;
                new_el->data = element; new_el->next = NULL; //change values in Node new_el
                tail = new_el;
                point->next = new_el; //previous object points to new_el
                ++nodeCount;
            }
		}

		//--------------------------------------------------

		T pop_front() 
		{
			if(size() == 0)
				throw std::invalid_argument("Cannot remove element from an empty list");
			link point = head;
			T object_data = point->data;
			if(size() == 1)
			{
				head = NULL; tail = NULL;
			}
			else
			{
				head = head->next;
			}
			addNode(point);
			--nodeCount;
			return object_data;
		}

		//--------------------------------------------------

		T pop_back() 
		{
			if(size() == 0)
				throw std::invalid_argument("Cannot remove element from an empty list");

			link point = tail;
			T object_data = point->data;
			if(size() == 1) 
			{
				head = NULL; tail = NULL;
			}
			else
			{
				link curr = head;
				do
                {
                    curr = curr->next;
                } while (curr->next != tail);
                curr->next = NULL;
			}
			addNode(point);
			--nodeCount;
			return object_data;
		}

		//--------------------------------------------------

		T remove ( int position ) 
		{
			if ( position >= nodeCount )
				throw std::out_of_range("Cannot remove an object outside of range!");
			if (position < 0)
				throw std::out_of_range("Cannot remove an object at a negative position");

			link point = head, prev = NULL;
			int count = position;

			while(count-- != 0)
			{
				prev = point;
				point = point->next;
			}

			T object_data = point->data;
			if(prev != NULL)
				prev->next = point->next;
			else
				head = point->next;

			addNode(point);
			--nodeCount;
			return object_data;
		}

		//--------------------------------------------------

		T item_at ( int position) 
		{
			if ( position >= nodeCount )
				throw std::out_of_range("Cannot retrieve an object outside of range!");
			if (position < 0)
				throw std::out_of_range("Cannot retrieve an object at a negative position");

			int count = position;
			link point = head;
			while(count-- != 0)
				point = point->next;
			return point->data;
		}

		T& operator[](int i) 
        {
        	if ( i >= nodeCount )
				throw std::out_of_range("Cannot retrieve an object outside of range!");
			if (i < 0)
				throw std::out_of_range("Cannot retrieve an object at a negative position");

            int count = i;
            link point = head;
            while (count-- != 0)
                point = point->next;
            return point->data;
        }

        T const& operator[](int i) const
        {
        	if ( i >= nodeCount )
				throw std::out_of_range("Cannot retrieve an object outside of range!");
			if (i < 0)
				throw std::out_of_range("Cannot retrieve an object at a negative position");
			
            int count = i;
            link point = head;
            while (count-- != 0)
                point = point->next;
            return point->data;
        }

		//--------------------------------------------------

		bool is_empty() const 
		{
			if(head == NULL && tail == NULL)
				return true;
			return false;
		}

		//--------------------------------------------------

		size_t size() const 
		{
			return nodeCount;
		}

		//--------------------------------------------------

		void clear() 
		{
			for(link t = head; t != 0; head = t)
            {
                t = head->next; //set pointer t to head
                delete head; //delete node
            }
            head = NULL;
            tail = NULL;
            nodeCount = 0;
		}
		//--------------------------------------------------

		bool contains ( const T& element, bool equals( const T& a, const T& b )) const 
		{
			link point = head;
            while(point != NULL)
            {
                if(equals(element, point->data))
                {
                    return true;
                }
                point = point->next;
            }
            return false;
		}

		//--------------------------------------------------

		std::ostream& print( std::ostream& out ) const 
		{
			if(is_empty()) {
                out << "<empty list>";
                return out;
            }
            else {
                link point = head;
                out << "[";
                while(point->next != NULL)
                {
                    out << point->data << ", ";
                    point = point->next;
                }
                T temp = point->data;
                out << temp << "]";
                return out;
            }
		}

		iterator begin() { return PSLL_Iter( head ); }
        iterator end() { return PSLL_Iter(); }
     
        const_iterator begin() const { return PSLL_Const_Iter( head ); }
        const_iterator end() const { return PSLL_Const_Iter(); }

	private:
		link head, tail, freeList;
		int nodeCount, freeCount;
		void addNode( Node* a )
		{
			if(freeList == NULL)
			{
				freeList = a;
				++freeCount;
			}
			else
			{
				a->next = freeList; 
				freeList = a;
				++freeCount;
			}
		}
		Node* getNode()
		{
			link point = freeList;
			if(freeCount > 1)
			{
				freeList = freeList->next;
			}
			else
			{
				freeList = NULL;
			}
			--freeCount;
			return point;
		}
		void checkSize()
		{
			//insert operations for pool-size checking
			if(nodeCount >= 100 && freeCount > (nodeCount/2))
			{
				while(freeCount-- > (nodeCount/2))
				{
					link point = freeList;
					freeList = freeList->next;
					delete point;
				}
			}
		}
	};
}

#endif