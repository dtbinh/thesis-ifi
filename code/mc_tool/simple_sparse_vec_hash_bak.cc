// Distributed under GNU General Public License (see license.txt for details).
//
//  Copyright (c) 2007 Shai Shalev-Shwartz.
//  All Rights Reserved.
//=============================================================================
// File Name: simple_sparse_vec_hash.cc
// implements the functions of simple_sparse_vec_hash.h
//=============================================================================

//*****************************************************************************
// Included Files
//*****************************************************************************
#include "simple_sparse_vec_hash.h"

/*---------------------------------------------------------------------------*/
simple_sparse_vector::simple_sparse_vector(std::istream& is) : my_vec() {
  
	// read the number of elements
	int n = 0;
	is >> n;

	// read the elements
	for (int i=0; i<n ; ++i) {

		// read the pair (key,val)
		uint key;
		is >> key;
		double val;
		is >> val;

		// insert to the map
		my_vec.push_back(IndexValuePair(key,val));

	}

}
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
simple_sparse_vector::simple_sparse_vector(std::istringstream& is,int n) : my_vec() {
  
	// read the elements
	for (int i=0; i<n ; ++i) {

		// read the pair (key,val)
		uint key;
		is >> key;
		double val;
		is >> val;

		// insert to the map
		my_vec.push_back(IndexValuePair(key,val));

	}
}
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
void simple_sparse_vector::scale(double s) {
/*
	for( simple_sparse_vector_iterator it = my_vec.begin(); 
		 it != my_vec.end(); it++) {
		(*it).second *= s;
	}
*/
	uint N = my_vec.size();
	for(uint i = 0; i < N; i++)
	{
		my_vec[i].second *= s;
	}
	
}
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
uint simple_sparse_vector::max_index() {

	uint d=0;
/*
	for( simple_sparse_vector_iterator it = my_vec.begin(); 
		 it != my_vec.end(); it++) {
		if ((*it).first > d) d = (*it).first;
	}
*/
	uint N = my_vec.size();
	for(uint i = 0; i < N; i++)
	{
		if(my_vec[i].first > d)
			d = my_vec[i].first;
	}
	return d;
}
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
double simple_sparse_vector::snorm() {

	double output = 0.0;
/*
	for(simple_sparse_vector_iterator it = my_vec.begin(); 
		it != my_vec.end(); it++) {
		double tmp = (*it).second;
		output += tmp*tmp;
	}
*/
	uint N = my_vec.size();
	for(uint i = 0; i < N; i++)
	{
	    double tmp = my_vec[i].second;
		output += tmp*tmp;
	}

	return(output);
}
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
void simple_sparse_vector::make_binary() {
/*
	for(simple_sparse_vector_iterator it = my_vec.begin(); 
		it != my_vec.end(); it++) {
		(*it).second = 1.0;
	}
*/
	//version rising
	uint N = my_vec.size();
	for(uint i = 0; i < N; i++)
	{
	    my_vec[i].second = 1.0;
	}
}
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
void simple_sparse_vector::print(std::ostream& os) {
/*
	for(simple_sparse_vector_iterator it = my_vec.begin(); 
		it != my_vec.end(); it++) {
		os << "(" << (*it).first << "," << (*it).second << ") "; 
	}
*/
	uint N = my_vec.size();
	for(uint i = 0; i < N; i++)
	{
	    os << "(" << my_vec[i].first << "," << my_vec[i].second << ") "; 
	}
	os << std::endl;
}
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
void simple_sparse_vector::zero() {
	my_vec.clear();
}
/*---------------------------------------------------------------------------*/



#ifdef nodef

/*---------------------------------------------------------------------------*/
double simple_hash_table::get(uint i) {
 
	uint pos = i % my_vec.size();
/*
	for (simple_sparse_vector_iterator it = my_vec[pos].begin(); 
		 it != my_vec[pos].end(); ++it) {
		if ((*it).first == i) return (*it).second;
	}
*/
	uint N = my_vec[pos].size();
	for(uint j = 0; j < N; j++)
	{
		if(my_vec[pos][j].first == i) return my_vec[pos][j].second;
	}
	return 0.0;
}
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
double& simple_hash_table::get_ref(uint i) {

	uint pos = i % my_vec.size();
/*
	for (simple_sparse_vector_iterator it = my_vec[pos].begin(); 
		 it != my_vec[pos].end(); ++it) {
		if ((*it).first == i) return (*it).second;
	}
*/
	uint N = my_vec[pos].size();
	for(uint j = 0; j < N; j++)
	{
	    if(my_vec[pos][j].first == i) return my_vec[pos][j].second;
	}
	IndexValuePair tmp(i,0.0);
	my_vec[pos].push_back(tmp);
	simple_sparse_vector_iterator it = my_vec[pos].end()-1;
	return (*it).second;

}
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
void simple_hash_table::scale(double s) {
/*
	for (simple_hash_table_iterator it = my_vec.begin();
		 it != my_vec.end(); it++) {
		for (simple_sparse_vector_iterator it2 = (*it).begin();
			 it2 != (*it).end(); it2++) {
			(*it2).second *= s;
		}
	}
*/

	uint N = my_vec.size();
	for(uint i = 0; i < N; i++)
	{
	    uint sub_size = my_vec[i].size();
		for(uint j = 0; j < sub_size; j++)
			my_vec[i][j].second *= s;
		
	}
}
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
void simple_hash_table::scale_and_add(simple_sparse_vector& other, double a, double b) {
  
	scale(a); add(other,b);

}
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
void simple_hash_table::scale_and_add(simple_hash_table& other, double a, double b) {

	scale(a); add(other,b);

}
/*---------------------------------------------------------------------------*/

void simple_hash_table::add(simple_hash_table& other, double b) { 
/*
	for (simple_hash_table_iterator it = other.my_vec.begin();
		 it != other.my_vec.end(); it++) {
		for (simple_sparse_vector_iterator it2 = (*it).begin();
			 it2 != (*it).end(); it2++) {
			double& val = get_ref((*it2).first);
			val += (b * (*it2).second);
		}
	}
*/
	uint N = other.my_vec.size();
	for(uint i = 0; i < N; i++)
	{
	    uint sub_size = other.my_vec[i].size();
		for(uint j = 0; j < sub_size; j++)
		{
			double& val = get_ref(other.my_vec[i][j].first);
			val += (b * other.my_vec[i][j].second);
		}
	}
}

void simple_hash_table::add(simple_sparse_vector& other, double b) { 
/*
	for (simple_sparse_vector_iterator it = other.my_vec.begin();
		 it != other.my_vec.end(); it++) {
		double& val = get_ref((*it).first);
		val += (b * (*it).second);
	}
*/
	uint N = other.my_vec.size();
	for(uint i = 0; i < N; i++)
	{
		double& val = get_ref(other.my_vec[i].first);
		val += (b * other.my_vec[i].second);
	}
}


/*---------------------------------------------------------------------------*/
double simple_hash_table::snorm() {
/*
	double output = 0.0;
	for (simple_hash_table_iterator it = my_vec.begin();
		 it != my_vec.end(); it++) {
		for (simple_sparse_vector_iterator it2 = (*it).begin();
			 it2 != (*it).end(); it2++) {
			double tmp = (*it2).second;
			output +=  (tmp*tmp);
		}
	}
*/
	uint N = my_vec.size();
	for(uint i = 0; i < N; i++)
	{
	    uint sub_size = my_vec[i].size();
		for(uint j = 0; j < sub_size; j++)
		{
			double tmp = my_vec[i][j].second;
			output +=  (tmp*tmp);
		}
	}

	return(output);
}
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
void simple_hash_table::print(std::ostream& os) {
/*
	for (simple_hash_table_iterator it = my_vec.begin();
		 it != my_vec.end(); it++) {
		for (simple_sparse_vector_iterator it2 = (*it).begin();
			 it2 != (*it).end(); it2++) {
			os << "(" << (*it2).first << "," << (*it2).second << ") ";       
		}
	}
*/
	uint N = other.my_vec.size();
	for(uint i = 0; i < N; i++)
	{
	    uint sub_size = other.my_vec[i].size();
		for(uint j = 0; j < sub_size; j++)
		{
		    os << "(" << my_vec[i][j].first << "," << my_vec[i][j].second << ") ";  
		}
	}


	os << std::endl;
}
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
void simple_hash_table::zero() {

	uint m = my_vec.size();
	my_vec.clear();
	my_vec.resize(m);

}
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
double operator* (simple_hash_table& u, simple_sparse_vector& v) {
  
	double output = 0.0;
/*
	for(simple_sparse_vector_iterator it = v.my_vec.begin(); 
		it != v.my_vec.end(); it++) {
		output += (*it).second * u.get((*it).first);
	}
*/
	uint N = v.my_vec.size();
	for(uint i = 0; i < N; i++)
	{
		output += v.my_vec[i].second * u.get(v.my_vec[i].first);
	}

	return(output);
}
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
double operator* (simple_sparse_vector& u, simple_hash_table& v) { 
	return(v*u); 
}
/*---------------------------------------------------------------------------*/

#endif
