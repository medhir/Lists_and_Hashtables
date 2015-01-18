#ifndef _SDAL_H
#define _SDAL_H

//SDAL.h
//Simple Dynamic Array List

//Medhir Bhargava
//COP3530
//September 23, 2014


#include <iostream>
#include <stdexcept>

namespace cop3530
{
	template<typename T>
	class SDAL
	{
	private:
		int numElements;
		T* p = NULL; //pointer to type T
		int tail;
		void checkSize()
		{
			if(tail == (numElements-1))
			{
				numElements *= 1.5;
				T* temp = new T[numElements];
				for(int i = 0; i < (numElements / 1.5); ++i)
					temp[i] = p[i];
				delete [] p;
				p = temp;
			}
			if(numElements >= 100 && tail < (numElements/2))
			{
				numElements = numElements/2;
				T* temp = new T[numElements];
				for(int i = 0; i < numElements; ++i)
					temp[i] = p[i];
				delete [] p;
				p = temp;
			}
		}
	public:

		typedef std::size_t size_t;
        typedef T value_type;

		/*
        ITERATOR CLASS DEFINITIONS
        */

        class SDAL_Iter //: public std::<std::forward_iterator_tag, T>
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
            typedef SDAL_Iter self_type;
            typedef SDAL_Iter& self_reference;

        private: 
            T* current;

        public:
            explicit SDAL_Iter(T* start = NULL) : current(start) {}
            SDAL_Iter(const SDAL_Iter& src) : current(src.current) {}
            self_reference operator=(const SDAL_Iter& src)
            {
                current = src.current;
                return *this;
            }

            reference operator*() const
            {
                return *current;
            }

            pointer operator->() const
            {
                return current;
            }

            self_reference operator++()
            {
                ++current;
                return *this;
            }//preincrement

            self_reference operator++(int)
            {
                SDAL_Iter results(*this);
                ++current;
                return results;
            } //post increment

            bool operator==(const SDAL_Iter& rhs) const
            {
                return (current == rhs.current);
            }

            bool operator!=(const SDAL_Iter& rhs) const
            {
                return (current != rhs.current);
            }
        }; //end SDAL_Iter

        class SDAL_Const_Iter //: public std::<std::forward_iterator_tag, T>
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
            typedef SDAL_Const_Iter self_type;
            typedef SDAL_Const_Iter& self_reference;

        private:
            const T* current;

        public:
            explicit SDAL_Const_Iter(const T* start = NULL) : current( start ) {}
            SDAL_Const_Iter(const SDAL_Iter& src) : current( src.current ) {}
            SDAL_Const_Iter(const SDAL_Const_Iter& src) : current( src.current ) {}
            
            self_reference operator=( const SDAL_Iter& src ) 
            {
                current = src.current;
                return *this;
            }

            self_reference operator=( const SDAL_Const_Iter& src ) 
            {
                current = src.current;
                return *this;
            }

            reference operator*() const 
            {
                return *current;
            }

            pointer operator->() const 
            {
                return current; 
            }

            self_reference operator++()
            {
                ++current;
                return *this;
            } //preincrement

            self_type operator++(int) 
            {
                SDAL_Const_Iter results(*this);
                ++current;
                return results;
            } //postincrement

            bool operator==( const SDAL_Const_Iter& rhs ) const 
            {
                return (current == rhs.current);
            }
            bool operator!=( const SDAL_Const_Iter& rhs ) const 
            {
                return (current != rhs.current);
            }
        }; //end SDAL_Const_Iter

        typedef SDAL_Iter iterator;
        typedef SDAL_Const_Iter const_iterator;  

		SDAL()
		{
			numElements = 50;
			p = new T[numElements];
			/*for (auto i = 0; i < numElements; ++i)
				p[i] = NULL;*/
			tail = -1;
		}

		SDAL(int size)
		{
			numElements = size;
			p = new T[numElements];
			tail = -1;
		}

		SDAL(const SDAL& src) 
		{
			numElements = src.numElements;
			p = new T[numElements];
			for(auto i = 0; i < numElements; ++i)
				p[i] = src.p[i];
			tail = src.tail;
		}
		~SDAL()
		{
			delete [] p;
		}

		SDAL& operator=(const SDAL& src)
		{
			if(&src == this) //check for self-assignment
				return *this;
			numElements = src.numElements;
			delete [] p;
			p = new T[numElements];
			for(int i = 0; i < numElements; ++i)
				p[i] = src[i];
			return *this;
		}

		//member functions
		T replace(const T& element, int position)
		{
			if ( position > tail )
				throw std::out_of_range("Cannot replace an object outside of range!");
			if (position < 0)
				throw std::out_of_range("Cannot replace an object at a negative position");

			T objectData = p[position];
			p[position] = element;
			return position;
		}

		void insert(const T& element, int position)
		{
			if ( position > (tail+1) )
				throw std::out_of_range("Cannot place an object outside of range!");
			if (position < 0)
				throw std::out_of_range("Cannot place an object at a negative position");

			checkSize();
			if(position == 0 && is_empty()) 
			{
				p[position] = element; //seg fault
				tail = 0;
			}
			for(int i = tail; i >= position; --i) 
			{
				p[i+1] = p[i];
			}
			p[position] = element;
			++tail;
		}

		void push_front(const T& element) 
		{
			checkSize();
			if(is_empty())
			{
				p[0] = element;
				tail = 0;
			}
			else 
			{
				for(int i = tail; i >= 0; --i)
				{
					p[i+1] = p[i];
				}
				p[0] = element;
				++tail;
			}
		}

		void push_back(const T& element) 
		{
			checkSize();
			if(is_empty())
			{
				p[0] = element;
				tail = 0;
			}
			else
			{
				p[++tail] = element;
			}
		}

		T pop_front() 
		{
			if(size() == 0)
				throw std::invalid_argument("Cannot remove element from an empty list");

			T objectData = p[0];
			for(int i = 0; i <= tail; ++i)
				p[i] = p[i+1];
			--tail;
			checkSize();
			return objectData;
		}

		T pop_back() 
		{
			if(size() == 0)
				throw std::invalid_argument("Cannot remove element from an empty list");

			T objectData = p[tail];
			p[tail--] = NULL;
			checkSize();
			return objectData;
		}

		T remove (int position) 
		{
			if ( position > tail )
				throw std::out_of_range("Cannot remove an object outside of range!");
			if (position < 0)
				throw std::out_of_range("Cannot remove an object at a negative position");

			T objectData = p[position];
			int count = position;
			while(count != tail)
			{
				p[count]=p[count+1];
				++count;
			}
			--tail; //move tail back a position
			return objectData;
		}

		T item_at(int position) const 
		{
			if ( position > tail )
				throw std::out_of_range("Cannot retrieve an object outside of range!");
			if (position < 0)
				throw std::out_of_range("Cannot retrieve an object at a negative position");

			return p[position];
		}

		T& operator[](int i) 
        {
        	if ( i > tail )
				throw std::out_of_range("Cannot retrieve an object outside of range!");
			if (i < 0)
				throw std::out_of_range("Cannot retrieve an object at a negative position");

            return p[i];
        }

        T const& operator[](int i) const
        {
        	if ( i > tail )
				throw std::out_of_range("Cannot retrieve an object outside of range!");
			if (i < 0)
				throw std::out_of_range("Cannot retrieve an object at a negative position");
			
            return p[i];
        }

		bool is_empty() const 
		{
			if(tail == -1)
				return true;
			return false;
		}

		size_t size() const 
		{
			return tail+1;
		}

		void clear() 
		{
			delete [] p;
			p = new T[numElements];
			tail = -1;
		}

		bool contains(const T& element, bool equals(const T& a, const T& b)) const 
		{
			for(int i = 0; i < numElements; ++i)
			{
				if(equals(element, p[i]))
					return true;
			}
			return false;
		}

		std::ostream& print(std::ostream& out) const 
		{
			if(tail == -1)
			{
				out << "<empty list>" << std::endl;
				return out;
			}
			else
			{
				out << "[";
				for(int i = 0; i < tail; ++i)
					out << p[i] << ", ";
				out << p[tail] << "]" << std::endl;
				return out;
			}
		}

		iterator begin() { return SDAL_Iter( p ); }
        iterator end() { return SDAL_Iter(p + size()); }
     
        const_iterator begin() const { return SDAL_Const_Iter( p ); }
        const_iterator end() const { return SDAL_Const_Iter(p + size()); }

	};
}

#endif