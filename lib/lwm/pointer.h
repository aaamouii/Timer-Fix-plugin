///	The class of the LWM of the timerfix plugin.
///	Latest update 20/10/2018
///
///	This class ensures reliable pointer handling.
///	Simply put, it's called smart pointer.
///
///	Current file: pointer.h
///
///	Copyright (c) 2018, KashCherry

#pragma once

///	Internal includes needed for the class
/// ---------------------------------

#include <stdio.h>
#include <string.h>
#include <assert.h>

/// ----------------------------------

///	Unique namespace
namespace LWM
{

	template<class T>
	class local_ptr
	{
	private:
		///////////////////////////////////////////
		typedef T *internal_pointer_type;
		///////////////////////////////////////////

		///	The main class variable holding the pointer
		internal_pointer_type internal_pointer;
		///	Variable flag
		bool no_except;

	public:

		///	Default constructor, accepts a pointer as an argument.
		///	In this case, it's believed that you must create it yourself.
		///	Example: local_ptr(new Pointer);
		explicit local_ptr(internal_pointer_type input_pointer = nullptr)
		{
			if (input_pointer)
			{
				no_except = true;
			}
			internal_pointer = input_pointer;
		}

		///	Default constructor, free up memory.
		~local_ptr()
		{
			if (internal_pointer && no_except)
			{
				delete internal_pointer;
			}
			no_except = false;
		}

		internal_pointer_type get() const
		{
			assert(internal_pointer != nullptr);
			return internal_pointer;
		}

		void reset(internal_pointer_type input_external_pointer = nullptr)
		{
			if (internal_pointer && no_except)
			{
				delete internal_pointer;
			}
			if (!input_external_pointer)
			{
				no_except = false;
			}
			internal_pointer = input_external_pointer;
		}

		/// Swaps the contents of two pointers
		void swap(local_ptr &s_arg)
		{
			internal_pointer_type temp = internal_pointer;
			internal_pointer = s_arg.internal_pointer;
			s_arg.internal_pointer = temp;
			
		}

		/// Returns true if the internal pointer is null
		bool empty()
		{
			return (internal_pointer == nullptr);
		}

		/// Operator functions
		/// ----------------------------------------
		internal_pointer_type operator->() const
		{
			assert(internal_pointer != nullptr);
			return internal_pointer;
		}
		T operator*() const
		{
			assert(internal_pointer != nullptr);
			return *internal_pointer;
		}
		bool operator>(internal_pointer_type pointer_to_result)
		{
			return (internal_pointer > pointer_to_result);
		}
		bool operator<(internal_pointer_type pointer_to_result)
		{
			return (internal_pointer < pointer_to_result);
		}
		bool operator>=(internal_pointer_type pointer_to_result)
		{
			return (internal_pointer >= pointer_to_result);
		}
		bool operator<=(internal_pointer_type pointer_to_result)
		{
			return (internal_pointer <= pointer_to_result);
		}
		/// ----------------------------------------

	};
	
}