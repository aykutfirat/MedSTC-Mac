# Makefile for MedSTC 1/5/2013 
# Original code by Jun Zhu junzhu [at] cs.cmu.edu
# Port to Unix by Aykut Firat aykutfirat [at] gmail.com
# Use the following to compile under Linux/Mac
all: medstc

clear:
	rm -f ../SVMLight/*.o; 
	rm -f ../SVM_Multiclass/*.o;
	rm -f *.o *.exe MedSTC;
	
svm_light_clean: 
	cd ../SVMLight; g++ -c *.cpp

svm_struct_clean: 
	cd ../SVM_Multiclass; g++ -c *.cpp

medstc_clean: 
	g++ -c *.cpp

medstc: svm_light_clean svm_struct_clean medstc_clean
	g++ ../SVMLight/*.o ../SVM_Multiclass/*.o *.o -o MedSTC
