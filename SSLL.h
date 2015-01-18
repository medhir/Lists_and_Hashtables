#ifndef _SSLL_H_
#define _SSLL_H_

// SSLL.H
//
// Singly-linked list (non-polymorphic)
//
// by Dave Small
// for COP 3530
// 201409.16 - created

#include <iostream>
#include <stdexcept>

namespace cop3530 {
    
    template <typename T>
    class SSLL {
    public:

        typedef std::size_t size_t;
        typedef T value_type;

        //--------------------------------------------------
        // Constructors/destructor/assignment operator
        //--------------------------------------------------
        struct Node {
            T data;
            Node *next;
            
            Node (T value, Node *t)
            {
                data = value;
                next = t;
            }
        }; // end struct Node
        typedef Node *link;

        /*
        ITERATOR CLASS DEFINITIONS
        */

        class SSLL_Iter //: public std::<std::forward_iterator_tag, T>
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
            typedef SSLL_Iter self_type;
            typedef SSLL_Iter& self_reference;

        private: 
            Node* current;
            
        public:
            explicit SSLL_Iter(Node* start = NULL) : current(start) {}
            SSLL_Iter(const SSLL_Iter& src) : current(src.current) {}
            self_reference operator=(const SSLL_Iter& src)
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
                SSLL_Iter results(*this);
                current = current->next;
                return results;
            } //post increment

            bool operator==(const SSLL_Iter& rhs) const
            {
                return (current == rhs.current);
            }

            bool operator!=(const SSLL_Iter& rhs) const
            {
                return (current != rhs.current);
            }
        }; //end SSLL_Iter

        class SSLL_Const_Iter //: public std::<std::forward_iterator_tag, T>
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
            typedef SSLL_Const_Iter self_type;
            typedef SSLL_Const_Iter& self_reference;

        private:
            const Node* current;

        public:
            explicit SSLL_Const_Iter(const Node* start = NULL) : current( start ) {}
            SSLL_Const_Iter(const SSLL_Iter& src) : current( src.current ) {}
            SSLL_Const_Iter(const SSLL_Const_Iter& src) : current( src.current ) {}
            
            self_reference operator=( const SSLL_Iter& src ) 
            {
                current = src.current;
                return *this;
            }

            self_reference operator=( const SSLL_Const_Iter& src ) 
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
                SSLL_Const_Iter results(*this);
                current = current->next;
                return results;
            } //postincrement

            bool operator==( const SSLL_Const_Iter& rhs ) const 
            {
                return (current == rhs.current);
            }
            bool operator!=( const SSLL_Const_Iter& rhs ) const 
            {
                return (current != rhs.current);
            }
        }; //end SSLL_Const_Iter

        typedef SSLL_Iter iterator;
        typedef SSLL_Const_Iter const_iterator;  
        
        SSLL(): head(NULL), tail(NULL), nodeCount(0) {}
        
        //--------------------------------------------------
        SSLL( const SSLL& src ): head(NULL), tail(NULL), nodeCount(0)
        {
            link currentNode = src.head;
            while(currentNode != NULL)
            {
                push_back(currentNode->data); //Add data from src's node
                currentNode = currentNode->next; //move pointer position up a node
            }
        }
        
        //--------------------------------------------------
        ~SSLL() {
            // safely dispose of this SSLL's contents
            for(link t = head; t != 0; head = t)
            {
                t = head->next; //set pointer t to head
                delete head; //delete node
            }
            head = NULL; tail = NULL;
        }
        
        //--------------------------------------------------
        SSLL& operator=( const SSLL& src )
        {
            if ( &src == this ) // check for self-assignment
            {
                return &this;   // do nothing
            }
            // safely dispose of this SSLL's contents
            ~cop3530::SSLL<T>();
            // populate this SSLL with copies of the other SSLL's contents
            link currentNode = src.head;
            while(currentNode != NULL)
            {
                push_front(currentNode->data);
                currentNode = currentNode->next;
            }
            return *this;
        }
        
        //--------------------------------------------------
        // member functions
        //--------------------------------------------------
        
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
        void insert( const T& element, int position )
        {
            if ( position > nodeCount )
                throw std::out_of_range("Cannot place an object outside of range!");
            if (position < 0)
                throw std::out_of_range("Cannot place an object at a negative position");

            //allocate memory for the new element to be inserted into list
            link new_el = new Node(element, NULL);
            link point = head, prev = NULL; //temp points to first object, prev to keep track of "previous" object
            int count = position;
            
            if (count == 0 && is_empty())
            {
                head = new_el;
                tail = new_el;
                ++nodeCount;
            }
            else if (count == 0)
            {
                new_el->next = head; //new element points to object being pushed back
                head = new_el; //head points to new element
                ++nodeCount;
            }
            else
            {
                while(point != NULL && count-- != 0)
                {
                    prev = point;
                    point = point->next;
                }
                
                //prev needs to point to new object.
                prev->next = new_el;
                new_el->next = point;
                ++nodeCount;
            }
        }
        
        //--------------------------------------------------
        void push_front( const T& element )
        {
            if(is_empty())
            {
                link new_el = new Node(element, NULL);
                head = new_el;
                tail = new_el;
                ++nodeCount;
            }
            else
            {
                link new_el = new Node(element, head); //create new Node, store data and pointer values
                new_el->next = head;
                head = new_el; //head now points to new_el;
                ++nodeCount;
            }
        }
        
        //--------------------------------------------------
        void push_back( const T& element )
        {
            link new_el = new Node(element, NULL);
            if(is_empty())
            {
                head = new_el;
                tail = new_el;
                ++nodeCount;
            }
            else
            {
                link point = tail;
                point->next = new_el; //previous object points to new_el
                tail = new_el;
                ++nodeCount;
            }
        }
        
        //--------------------------------------------------
        T pop_front()
        {
            if(size() == 0)
                throw std::invalid_argument("Cannot remove element from an empty list");

            if(size() == 1)
            {
                link point = head;
                T object = point->data;
                delete head;
                //head = NULL;
                tail = NULL;
                --nodeCount;
                return object;
            }
            else
            {
                link point = head;
                T object = head->data;
                head = head->next;
                delete point;
                --nodeCount;
                return object;
            }
        }
        
        //--------------------------------------------------
        T pop_back()
        {
            if(size() == 0)
                throw std::invalid_argument("Cannot remove element from an empty list");

            T object = tail->data;
            if(size() == 1)
            {
                T object = head->data;
                delete head;
                head = NULL, tail = NULL;
                --nodeCount;
                return object;
            }
            else
            {
                T object = tail->data;
                link point = head;
                while(point->next != tail)
                {
                    point = point->next;
                }
                point->next = NULL;
                delete tail;
                tail = point;
                --nodeCount;
                return object;
            }
        }
        
        //--------------------------------------------------
        T remove( int position )
        {
            if ( position >= nodeCount )
                throw std::out_of_range("Cannot remove an object outside of range!");
            if (position < 0)
                throw std::out_of_range("Cannot remove an object at a negative position");

            if(position == 0)
            {
                return pop_front();
            }
            else 
            {
                link prev = NULL, point = head;
                int count = position;
                while(count-- != 0)
                {
                    prev = point;
                    point = point->next;
                }
                prev->next = point->next;
                T data = point->data;
                delete point;
                --nodeCount;
                return data;
            }
        }
        
        //--------------------------------------------------
        T item_at( int position ) const
        {
            if ( position >= nodeCount )
                throw std::out_of_range("Cannot retrieve an object outside of range!");
            if (position < 0)
                throw std::out_of_range("Cannot retrieve an object at a negative position");

            int count = position;
            link point = head;
            while (count-- != 0)
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
            if(head == NULL && tail == NULL) //return true if head points to NULL
                return true;
            return false; //else not empty
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
        bool contains( const T& element,
                      bool equals( const T& a, const T& b  ) ) const
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

        iterator begin() { return SSLL_Iter( head ); }
        iterator end() { return SSLL_Iter(); }
     
        const_iterator begin() const { return SSLL_Const_Iter( head ); }
        const_iterator end() const { return SSLL_Const_Iter(); }
        
    private:
        link head, tail;
        int nodeCount;
        
    }; //end class SSLL
} // end namespace cop3530

#endif // _SSLL_H_