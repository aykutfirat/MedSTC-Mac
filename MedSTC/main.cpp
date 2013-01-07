// (C) Copyright 2011, Jun Zhu (junzhu [at] cs [dot] cmu [dot] edu)

// This file is part of MedSTC.

// MedSTC is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 of the License, or (at your
// option) any later version.

// MedSTC is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA

#include <sys/stat.h>
#include <sys/types.h>
#include <vector>
#include <map>
#include <string>
#include "../SVMLight/svm_common.h"
#include "MedSTC.h"
#include "Corpus.h"
using namespace std;

void read_param( char *filename, vector<double> &vecC, vector<double> &vecEll );
void read_dir( char *filename, vector<char *> &dir_list );
void read_voc(char *fileName, vector<string> &vecStr );
void find_repwords( char *filename, Corpus *pC, int topN, Params *param, vector<string> &vocab );

int main(int argc, char* argv[])
{
	seedMT( time(NULL) );

	if ( argc > 1 )
	{
		Corpus* c = new Corpus();
		Params param;

		param.read_settings( argv[5] ); //"settings.txt");
		param.NTOPICS    = atoi(argv[1]);
		param.INITIAL_C  = atof(argv[2]);
		param.LAMBDA     = atof(argv[3]);
		param.RHO        = atof(argv[4]);
		param.NFOLDS	 = 1;
		if ( argc > 6 ) param.NFOLDS     = atoi(argv[6]);
		if ( argc > 7 ) param.DELTA_ELL  = atof(argv[7]);

		c->read_data(param.train_filename, param.NLABELS);
		char dir[512];
		sprintf(dir, "s%d_c%d_f%d_s%d", param.NTOPICS, (int)param.INITIAL_C, 
			param.NFOLDS, param.SUPERVISED);
		mkdir(dir,0755);

		MedSTC model;
		model.train("random", dir, c, &param);

		// testing.
		Corpus *tstC = new Corpus();
		tstC->read_data(param.test_filename, param.NLABELS);
		MedSTC evlModel;
		double dAcc = evlModel.sparse_coding(dir, tstC, &param);
		printf("Accuracy: %.3f\n", dAcc);

		delete tstC;
		delete c;
	} else {
		printf("usage : MedSTC est [initial alpha] [k] [labels] [random/seeded/*] [directory]\n");
		printf("        MedSTC cv [foldnum] [foldix] [initial alpha] [k] [labels] [settings] [data] [random/seeded/*] [directory]\n");
		printf("        MedSTC inf [settings] [model] [data] [name]\n");
	}

	return 0;
}

/* find the topc N representative words for each class. */
void find_repwords( char *filename, Corpus *pC, int topN, Params *param, vector<string> &vocab )
{
	vector<vector<int> > classWrdCount( param->NLABELS );
	for ( int k=0; k<param->NLABELS; k++ ) {
		classWrdCount[k].resize( pC->num_terms, 0 );
	}
	for ( int d=0; d<pC->num_docs; d++ ) {
		Document *pDoc = &(pC->docs[d]);
		for ( int n=0; n<pDoc->length; n++ ) {
			int wrd = pDoc->words[n];
			int wcount = pDoc->counts[n];
			int gndLabel = pDoc->gndlabel;

			classWrdCount[gndLabel][wrd] += wcount;
		}
	}

	FILE *fptr = fopen( filename, "w" );
	for ( int k=0; k<param->NLABELS; k++ ) {
		for ( int i=0; i<topN; i++ ) {
			int maxIx = -1, maxCount = -1;
			for ( int n=0; n<pC->num_terms; n++ ) {
				int nCount = classWrdCount[k][n];
				if ( maxCount < nCount ) {
					maxIx = n; maxCount = nCount;
				}
			}
			fprintf( fptr, "%s ", vocab[maxIx].c_str() );
			classWrdCount[k][maxIx] = -1;
		}
		fprintf( fptr, "\n" );
	}
	fclose( fptr );
}

void read_voc(char *fileName, vector<string> &vecStr )
{
	FILE *fileptr = fopen(fileName, "r");
	char buff[512];
	fscanf(fileptr, "%s %s %s %s %s\n", buff, buff, buff, buff, buff);
	while ( (fscanf(fileptr, "%s", buff) != EOF) ) {
		vecStr.push_back( string( buff ) );
		fscanf(fileptr, "%s %s %s %s\n", buff, buff, buff, buff);
	}
	fclose( fileptr );
}

void read_dir( char *filename, vector<char *> &dir_list )
{
	FILE *fptr = fopen( filename, "r" );
	char buff[512];
	while ( fscanf(fptr, "%s\n", buff) != EOF ) {
		char *dir = new char[512];
		sprintf(dir, "%s", buff);
		dir_list.push_back( dir );
	}
	fclose( fptr );
}

void read_param( char *filename, vector<double> &vecC, vector<double> &vecEll )
{
	FILE *fptr = fopen( filename, "r" );
	int nNumC, nNumD;
	double dVal = 0;
	vector<double> c, d;
	fscanf(fptr, "%d", &nNumC);
	for ( int i=0; i<nNumC; i++ ) {
		fscanf(fptr, "%lf", &dVal);
		c.push_back( dVal );
	}

	fscanf(fptr, "%d", &nNumD);
	for ( int i=0; i<nNumD; i++ ) {
		fscanf(fptr, "%lf", &dVal);
		d.push_back( dVal );
	}
	fclose( fptr );

	vecC.resize( nNumC * nNumD );
	vecEll.resize( nNumC * nNumD );
	int ix = 0;
	for ( int i=0; i<nNumC; i++ ) {
		for ( int j=0; j<nNumD; j++ ) {
			vecC[ix] = c[i];
			vecEll[ix] = d[j];
			ix ++;
		}
	}
}
