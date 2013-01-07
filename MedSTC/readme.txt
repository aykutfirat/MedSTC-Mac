***************************
STC: Sparse Topical Coding
***************************

Jun Zhu
junzhu [at] cs.cmu.edu
Port to Mac by Aykut Firat aykutfirat@gmail.com

(C) Copyright 2011, Jun Zhu (junzhu [at] cs [dot] cmu [dot] edu)

This file is part of STC.

STC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2 of the License, or (at your
option) any later version.

STC is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
USA

------------------------------------------------------------------------

This is a C implementation of Sparse Topical Coding (STC), a model 
of discrete data which is fully described in Zhu et al. (2011)
(http://www.cs.cmu.edu/~junzhu/stc/stc.pdf).

------------------------------------------------------------------------

TABLE OF CONTENTS

A. COMPILING

B. SETTINGS FILE

C. DATA FILE FORMAT

D. ESTIMATION AND INFERENCE

E. QUESTIONS, COMMENTS, PROBLEMS, UPDATE ANNOUNCEMENTS

------------------------------------------------------------------------

A. COMPILING

   $cd MedSTC 
   $make
   #$make clear to remove object and executable files

------------------------------------------------------------------------

B. Settings file

See settings.txt for a sample. These are placeholder values; they
should be experimented with.

This is of the following form:

     supervised [0 or 1]
     primal svm [0 or 1, default 1]
     var max iter [integer e.g., 20]
     var convergence [float e.g., 1e-4]
     em max iter [integer e.g., 100]
     em convergence [float e.g., 1e-4]
     model C [positive float e.g., 0.5]
     delta ell [positive float e.g., 3600]
     lambda [positive float e.g., 0.1]
     rho [positive float e.g., 0.01]
     svm_alg_type [0 or 2]
     train_file: [string e.g., ..\train.dat]
     test_file: [string e.g., ..\test.dat]
     class_num: [20 for 20 Newsgroup]
     res_file: overall-res_supervised.txt

where the settings are

     [supervised]
     
     If the value is "1", the model is a supervised MedSTC; if "0", the model 
     is the unsupervised STC.
     
     [primal svm]
     
     Only works when "supervised" is set at 1. If the value is "1", use the
     loss-augmented prediction (i.e., sub-gradient) to update document codes;
     use the gradient with Lagrangian multipliers to update document codes.
     
     [var max iter]

     The maximum number of iterations of coordinate descent for a single document.

     [var convergence]

     The convergence criteria for coordinate descent.  Stop if
     (objective_old - objective) / abs(objective_old) is less than this value (or
     after the maximum number of iterations). Note that "objective" is the objective 
     value for a single document.

     [em max iter]

     The maximum number of iterations of hierarchical sparse coding, dictionary learning, 
     and svm training (for supervised MedSTC).

     [em convergence]

     The convergence criteria for coordinate descent. Stop if (objective_old -
     objective) / abs(objective_old) is less than this value (or after the
     maximum number of iterations).  Note that "objective" is the objective value 
     for the whole corpus.

     [delta ell]
     
     The parameter for the svm cost function, i.e., 0/(delta ell) loss.
     
     [C],[lambda],[rho]

     These are the regularization constants.
      
     [train_file]
     
     The file name of training data.
     
     [test_file]
     
     The file name of testing data.

     [class_num]
     
     The number of class labels for the classification problem.
     
     [res_file]
     
     The name of the file for saving prediction results.


2. Data format

Under STC, the words of each document are assumed exchangeable. Thus, each document 
is succinctly represented as a sparse vector of word counts. The data is a file 
where each line is of the form:

     [M] [label] [term_1]:[count] [term_2]:[count] ... [term_M]:[count]

where [M] is the number of unique terms in the document; [label] is the true label
of the document; the [count] associated with each term is how many times that term 
appeared in the document. Note that [term_1] is an integer which indexes the
term; it is not a string.

------------------------------------------------------------------------

D. ESTIMATION AND INFERENCE

For simplicity, a command is provided for doing both estimation and inference.  
Usage is:

     MedSTC [K] [C] [lambda] [rho] [setting] [fold] [delta ell]
     
     where [setting] is the name of setting file (e.g., "settings_20ng.txt"); 
     and [fold] is an integer used only for naming the directory for saving models.
     
     e.g.
     MedSTC 20 0.5 0.1 0.01 settings_20ng.txt

------------------------------------------------------------------------

E. QUESTIONS, COMMENTS, PROBLEMS, AND UPDATE ANNOUNCEMENTS

Questions, comments, and problems should be addressed to: junzhu@cs.cmu.edu.

Update announcements will be posted at: http://cs.cmu.edu/~junzhu/stc.htm
