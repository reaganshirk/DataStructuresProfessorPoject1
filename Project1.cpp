//**********************************
// Aditya Narasimhan
// Project 1 CS 2413
// Spring 2019
//**********************************

#include <iostream>
using namespace std;

class SparseRow {
	protected:
		int row; //Row#
		int col; //Column#
		int value; //We will assume that all our values will be integers
	public:
		SparseRow (); //default constructor; row=-1;col=-1;value=0 
		void display(); // print Row#, Column#, value
		void setRow (int r);
		void setCol (int c);
		void setValue (int v);
		int getRow ();
		int getCol ();
		int getValue ();
		//other methods as you deem fit
};

SparseRow::SparseRow () {
	row = -1;
	col = -1;
	value = 0;
}

void SparseRow::display() {
	cout << row << ", " << col << ", " << value << endl;
}

void SparseRow::setRow (int r) {
	row = r;
}

void SparseRow::setCol (int c) {
	col = c;
}

void SparseRow::setValue (int v) {
	value = v;
}

int SparseRow::getRow () {
	return row;
}

int SparseRow::getCol () {
	return col;
}

int SparseRow::getValue () {
	return value;
}

class SparseMatrix {
	protected:
		int noRows; //Number of rows of the original matrix
		int noCols; //Number of columns of the original matrix
		int commonValue; //read from input
		int noNonSparseValues;
		SparseRow* myMatrix; //Array
	public:
		SparseMatrix ();
		SparseMatrix (int n, int m, int cv, int noNSV);
		void setSparseRow (int pos, int r, int c, int v);
		SparseMatrix* Transpose (); //Matrix Transpose
		SparseMatrix* Multiply (SparseMatrix& M); //Matrix Multiply
		SparseMatrix* Add (SparseMatrix& M); //Matrix Add
		void display();//Display the sparse matrix
		void displayMatrix (); //Display the matrix in its original format
		int getRows();
		int getCols();
		int getValWithRowandCol(int row, int col);
		void setnoNSV(int noNSVlocal);
		//other methods as you deem fit	
};

SparseMatrix::SparseMatrix () {
	noRows = 0;
	noCols = 0;
	commonValue = -1;
	noNonSparseValues = 0;
	myMatrix = NULL;
}

SparseMatrix::SparseMatrix (int n, int m, int cv, int noNSV) {
	noRows = n;
	// cout << "numRows set as:" << noRows << endl;
	noCols = m;
	// cout << "numCols set as:" << noCols << endl;
	commonValue = cv;
	noNonSparseValues = noNSV;
	myMatrix = new SparseRow[noNSV];
}

int SparseMatrix::getRows(){
	return noRows;
}

int SparseMatrix::getCols(){
	return noCols;
}

void SparseMatrix::setSparseRow (int pos, int r, int c, int v) {
	myMatrix[pos].setRow(r);
	myMatrix[pos].setCol(c);
	myMatrix[pos].setValue(v);
}

int SparseMatrix::getValWithRowandCol(int row, int col){
	for (int i = 0; i < noNonSparseValues; i++) {
        SparseRow x = myMatrix[i];
        if (x.getRow() == row && x.getCol() == col) {
             return x.getValue();
        }
    }
    return commonValue;
}

void SparseMatrix::setnoNSV (int noNSVlocal) {
	noNonSparseValues = noNSVlocal;
}

void SparseMatrix::displayMatrix () { // to print in matrix form
	for(int i = 0; i < this->getRows(); i++){
		for(int j = 0; j < this->getCols(); j++){
			cout << getValWithRowandCol(i, j) << "\t";
		}
		cout << endl;
	}
}

SparseMatrix* SparseMatrix::Transpose() {
	// initialize the resultant matrix
	SparseMatrix* transposed = new SparseMatrix (noRows,noCols,commonValue,noNonSparseValues);
	for (int i=0; i < noNonSparseValues; i++) 
	{
		((*transposed).myMatrix[i]).setRow (myMatrix[i].getCol()); //copy the col of A to row of result
		((*transposed).myMatrix[i]).setCol (myMatrix[i].getRow()); //copy the row of A to col of result
		((*transposed).myMatrix[i]).setValue (myMatrix[i].getValue()); //copy the value of A to value of result
	}
	return transposed;
}

SparseMatrix* SparseMatrix::Add(SparseMatrix& one) {
	// initializing the new resultant matrix
	SparseMatrix* sumMatrix = new SparseMatrix (this->getRows(),this->getCols(),this->commonValue+one.commonValue,this->noNonSparseValues + one.noNonSparseValues);
	int sumVal = 0;
	int position = 0;
	// bool flagArray[one.noNonSparseValues]; 
	bool* flagArray = new bool[one.noNonSparseValues]; // parallel bool array to maintain along with SparseRow table of B
	bool flagFound = false;
	
	int rowFirstOne;
	int columnFirstOne;
	int resultFirstOne;
	int rowSecondOne;
	int columnSecondOne;
	int resultSecondOne;


	//setting all of them to false to begin with
	for(int k = 0; k<one.noNonSparseValues; k++)
		flagArray[k] = false;


	for(int i = 0; i<this->noNonSparseValues; i++) //looping through A's sparseRow table
	{
		rowFirstOne = this->myMatrix[i].getRow();
		columnFirstOne = this->myMatrix[i].getCol(); 
		resultFirstOne = this->myMatrix[i].getValue();

		for(int j = 0; j<one.noNonSparseValues; j++) //looping through B's sparseRow table
		{
			rowSecondOne = one.myMatrix[j].getRow();
			columnSecondOne = one.myMatrix[j].getCol();  
			resultSecondOne = one.myMatrix[j].getValue();

			if(rowFirstOne == rowSecondOne && columnFirstOne == columnSecondOne) //comparing the rows and the cols of both A and B
				{
					sumVal = resultFirstOne + resultSecondOne; //adding the sum of two if they match
					flagArray[j] = true;
					flagFound = true;

					if(sumVal != sumMatrix->commonValue) //checking if the result is not the commonvalue of the resultant matrix
					{
						(*sumMatrix).setSparseRow (position, rowFirstOne, columnFirstOne, sumVal); //adding it to the sparseRow of the resultant matrix
						position++;
					}
					// sumVal = 0;
				} //if
		} //for j
		if(flagFound == false) // this happens if value exists in A's table but not B's
		{
			sumVal = this->commonValue + resultFirstOne; //we add it with the common value and then the check and then add to sparseRow table of result
			if(sumVal != sumMatrix->commonValue)
			{
				(*sumMatrix).setSparseRow(position, rowFirstOne, columnFirstOne, sumVal);
				position++;
			}
			// sumVal = 0;
		}
		flagFound = false;
	} //for i


	// this is for if a value if existent in B's table but not A's
	for(int k = 0; k<one.noNonSparseValues; k++)
	{
		if(flagArray[k] == false)
		{
			resultSecondOne = one.myMatrix[k].getValue();
			sumVal = this->commonValue + resultSecondOne; //we add it with the common value and then the check and then add to sparseRow table of result
			if(sumVal != sumMatrix->commonValue)
			{
				(*sumMatrix).setSparseRow(position, rowSecondOne, columnSecondOne, sumVal);
				position++;
			}
			// sumVal = 0;
		}
	}
	// reset the length of the sparseRow table of the result
	sumMatrix->setnoNSV(position);

	return sumMatrix;
}


SparseMatrix* SparseMatrix::Multiply(SparseMatrix& one) {
		SparseMatrix* multiplymatrix = new SparseMatrix(noRows, one.getCols(), this->commonValue*one.commonValue, this->noRows*one.getRows()*one.getCols());
		int counter = 0;

		for (int i = 0; i < (*multiplymatrix).getRows(); i++) // looping through the rows of A
		{
			for (int j = 0; j < (*multiplymatrix).getCols(); j++) // looping through the cols of B
			{
				int tempSum = 0;
				for (int k = 0; k < noNonSparseValues; k++) // looping through the SparseRow table of A
				{
					if (myMatrix[k].getRow() == i) // checking if the row of A matches with the myMatrix row from A
					{
						for (int l = 0; l < one.noNonSparseValues; l++) // looping through B's SparseRow table
						{
							if (one.myMatrix[l].getCol() == j) // checking if the col of B matches with the mymatrix col of B
							{
								if ((myMatrix[k].getCol() == one.myMatrix[l].getRow())) // checking if the myMatrix col of A matches with the myMatrix row of B
								{
									tempSum += myMatrix[k].getValue() * one.myMatrix[l].getValue(); //cummulative sum
								} //if col == row
							} //if col == j
						} //for l
					} //if row == i
				} //for k

				//Adding the sum to the resultant sparseMatrix
				if (tempSum != multiplymatrix->commonValue) 
				{
					multiplymatrix->setSparseRow(counter, i, j, tempSum);
					counter++;
				} //if tempSum == cv
			} //for j
		} //for i

		//resetting the length of the resultant matrix
		(*multiplymatrix).setnoNSV(counter);
		
		return multiplymatrix;
}



void SparseMatrix::display() {
	// cout << "Inside display numRows:" << this->noRows << endl;
	// cout << "Inside display numCols:" << this->noCols << endl;
	for (int i=0; i < noNonSparseValues; i++) {
		myMatrix[i].display();
	}
}

int main () {

	int n, m, cv, noNSV;
	SparseMatrix* temp;
	int v;
	int k;

	cin >> n >> m >> cv >> noNSV;
	SparseMatrix* firstOne = new SparseMatrix(n, m, cv, noNSV);
	// cout << n << endl << m << endl << cv << endl << noNSV << endl;

	//Write the Statements to read in the first matrix

	k = 0; //position in the matrix

	for (int i=0; i < n; i++) {
		for (int j=0; j < m; j++) {
			cin >> v;
			if (v != cv) {
				(*firstOne).setSparseRow (k, i, j, v);
				k++;
			}
		}
	}

	cout << "First one in sparse matrix format" << endl;
	(*firstOne).display();

	cout << "First one in normal matrix format" << endl;
	(*firstOne).displayMatrix();

	cin >> n >> m >> cv >> noNSV;
	SparseMatrix* secondOne = new SparseMatrix(n, m, cv, noNSV);
	// cout << n << endl << m << endl << cv << endl << noNSV << endl;

	//Write the Statements to read in the second matrix

	k = 0; //position in the matrix

	for (int i=0; i < n; i++) {
		for (int j=0; j < m; j++) {
			cin >> v;
			if (v != cv) {
				(*secondOne).setSparseRow (k, i, j, v);
				k++;
			}
		}
	}
	
	cout << "Second one in sparse matrix format" << endl;
	(*secondOne).display();

	cout << "Second one in normal matrix format" << endl;
	(*secondOne).displayMatrix();
	
	temp = (*firstOne).Transpose();
	cout << "After Transpose first one" << endl;
	(*temp).displayMatrix();

	temp = (*secondOne).Transpose();
	cout << "After Transpose second one" << endl;
	(*temp).displayMatrix();


	cout << "Multiplication of matrices in sparse matrix form:" << endl;
	temp = (*secondOne).Multiply(*firstOne);
	(*temp).display();
	// (*temp).displayMatrix();
	
	cout << "Addition of matrices in sparse matrix form:" << endl;
	temp = (*secondOne).Add(*firstOne);
	(*temp).display();
	// (*temp).displayMatrix();


}
