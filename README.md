Readme for hmmtagger

1. Compilation
a. To compile the source, run 'make' in the src folder. The generated program's name is hmmtagger.
b. To recompile the program, please run 'make clean' first.

2. Run
a. The input parameters of the program are as follows:
	  Usage: program -m0|-m1|-m2|-m|-s|-k trainning-file test-file
	         -m0 produce model file of  ML estimation for transition probability
	         -m1 produce model file of simple smoothing for transition probability
	         -m2 produce model file of katz backoff for transition probability
	         -m using ML estimation for transition probability
	         -s using simple smoothing for transition probability
	         -k using Katz backoff algorithm for transition probability
b. The output is on the standard output, you can redirect it to a file. The output format is: every line is a sentence, and the tag predicted for one word follows that word. The last number of each line is the logged probability of that sentence.


3. Evaluation
I wrote a perl script, evaluation.pl to evaluation the performance of the result:
  perl -w evaluation.pl <truth file> <predicted file>
The script prints out a list of line as follows:
    L:1 recommended[VBN] -> recommended[JJ]
    L:2 slash[NN] -> slash[VB]
    L:2 Chairman[NNP] -> Chairman[NN]
    L:2 Einar[NNP] -> UNKA[NN]
    L:2 Greve[NNP] -> UNKA[NN]
Each line gives the line number in the testing dataset, and the word-tag pair in the truth file, then the word-tag pair in the predicted file.
The last line give the total number of words of the testing dataset, the number of words that have wrong tags, and the accuracy of the result.

4. Predicted Result
The predicted result of each model is in the result folder.

5. Model files
The model file for each model is in the model folder. Each file contains two parts,seperated by an empty line: the first part gives the emmission probability, the second part gives the transition probability of each model. In the model file, the logged probability of a transition probability that is 0 is set to -10000.
