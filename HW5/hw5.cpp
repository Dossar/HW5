/* 
 * File:   hw5.cpp
 * Author: Roy Van Liew and Saqib Zahid
 *
 * Last updated on March 9th, 2014, 5:03 PM
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

class Matrix
{
public:
    Matrix(); // Default constructor
    Matrix(int diagvalue); // Constructor with parameters
    Matrix(const Matrix& M); // Copy constructor
    ~Matrix(); // Destructor
    friend ostream& operator <<(ostream& out, const Matrix& Mat);
    friend Matrix operator *(const Matrix& Mat1, const Matrix& Mat2);
    friend Matrix operator +(const Matrix& Mat1, const Matrix& Mat2);
    friend Matrix operator -(const Matrix& Mat1);
    friend Matrix operator -(const Matrix& Mat1, const Matrix& Mat2);
    friend bool operator ==(const Matrix& Mat1, const Matrix& Mat2);
    friend ifstream& operator >>(ifstream& in, Matrix& Mat);
    
private:
    int m; // Rows
    int n; // Columns
    int **data; // We don't know the size yet, so this is just two dimensional reference to memory
    
};

// Default constructor. Creates zero matrix.
Matrix::Matrix() {
    
    // Matrix dimensions must be positive.
    do {
        cout << "Input the dimensions of your matrix." << endl;
        cout << "Rows: ";
        cin >> m;
        cout << "Columns: ";
        cin >> n;
        if (m < 0 || n < 0)
            cout << "Cannot have negative dimensions, try again." << endl;
    } while (m < 0 || n < 0);
    
    // Make dynamic matrix. A matrix is an array of arrays.
    data = new int*[m]; // Allocate for each row.
    for (int i = 0; i < m ; i++ )
        data[i] = new int[n]; // Allocate for each column.
    
    // File the matrix with all zeros.
    int i, j;
    for (i = 0; i < m ; i++) {
        for (j = 0; j < n ; j++)
            data[i][j] = 0;
    }
   
}

// Constructor with parameters. Creates diagonal matrix.
Matrix::Matrix(int diagvalue) {
    
    // For a diagonal matrix its dimensions must be positive and a square.
    do {
        cout << "Input the dimensions of your matrix." << endl;
        cout << "Rows: ";
        cin >> m;
        cout << "Columns: ";
        cin >> n;
        if(m != n)
            cout << "You must put in a square matrix for diagonals." << endl;
        if( m<0 || n<0 )
            cout << "Cannot have negative dimensions, try again." << endl;
    } while ( m != n || m<0 || n<0 );
    
    // Make dynamic matrix. A matrix is an array of arrays.
    data = new int*[m];
    for (int i = 0; i < m ; i++ )
        data[i] = new int[n];
    
    // Make the diagonal matrix after inputting square dimensions.
    int i, j;
    for (i = 0; i < n; i++) {
        // Initialize entire row to zero first, then insert number into diagonal.
        for (j = 0; j < n; j++)
            data[i][j] = 0;
        j = 0; // Reinitialize j to zero so we can get the correct column in the diagonal
        j += i;
        data[i][j] = diagvalue;
    }

}

// Copy constructor
Matrix::Matrix(const Matrix& M)
{
    int i, j;
    for (i = 0; i < n ; i++) {
        for (j = 0; j < n ; j++)
            data[i][j] = M.data[i][j]; // Copy elements from our referenced matrix into our new matrix.
    }
    
}

// Destructor
Matrix::~Matrix()
{
    // Delete every row in the matrix.
    int i;
    for ( i = 0 ; i < m ; i++ )
        delete [] data[i];
    delete [] data;
}


// Overloaded input operator
ifstream& operator >>(ifstream& in, Matrix& Mat){
    
    int i, j;
    
    // Read in 9 elements from our file.
    for( i = 0 ; i < Mat.m ; i++ ){
        for( j = 0 ; j < Mat.n ; j++ ){
            // fin ignores whitespace and newlines
            // We also keep our position in the file with fin
            in >> Mat.data[i][j];
        }
    }
    
    return in;
    
}

// Overloaded output operator
ostream& operator <<(ostream& out, const Matrix& Mat){
    
    int i, j;
    //cout << "Matrix:" << endl;
    for (i = 0; i < Mat.m ; i++) {
        for (j = 0; j < Mat.n ; j++) {
            out << Mat.data[i][j] << " ";
        }
        out << endl; // Newline for next row
    }
    out << "\n";
    
}

// Overloaded multiplication operator
Matrix operator *(const Matrix& Mat1, const Matrix& Mat2){
    
    // First matrix columns must match rows of second matrix for multiplication.
    if( Mat1.n != Mat2.m ){
        cout << "Columns of first matrix do not match rows of second matrix." << endl;
        return Mat1;
    }
    // Getting past the if statement means we have valid multiplication dimensions.
    else {
        Matrix multMat;
        int sum = 0;
        int i, j, k;
        // i for Mat1 rows, j for Mat2 columns, k for Mat2 rows and Mat1 columns.
        // For each row of Mat1 and for each column of Mat2, multiply the matrices.
        // We get Mat1.m by Mat2.n dimensions product matrix after multiplication.
        for (i = 0; i < Mat1.m ; i++) {
            for (j = 0; j < Mat2.n ; j++) {
                sum = 0;
                // We traverse down the rows of said column in Mat2 to get the sum for the product matrix.
                for (k = 0; k < Mat2.m ; k++)
                    sum += Mat1.data[i][k] * Mat2.data[k][j];
                multMat.data[i][j] = sum;
            }
        }
        return multMat;
    }
    
}

// Overloaded addition operator
Matrix operator +(const Matrix& Mat1, const Matrix& Mat2){
    
    // Check that the dimensions of both matrices are equal first.
    if( ( Mat1.m != Mat2.m ) || ( Mat1.n != Mat2.n ) ){
        cout << "Matrices must be of equal dimensions for addition." << endl;
        return Mat1;
    }
    // Getting past the if statement means both matrices have the same dimensions.
    else {
        Matrix addMat;
        int sum = 0;
        int i, j;
        for (i = 0; i < Mat1.m; i++) {
            for (j = 0; j < Mat1.n; j++) {
                sum = Mat1.data[i][j] + Mat2.data[i][j];
                addMat.data[i][j] = sum;
            }
        }
        return addMat;
    }
    
}

// Overloaded binary minus (subtraction) operator
Matrix operator -(const Matrix& Mat1, const Matrix& Mat2){
    
    // Check that the dimensions of both matrices are equal first.
    if( ( Mat1.m != Mat2.m ) || ( Mat1.n != Mat2.n ) ){
        cout << "Matrices must be of equal dimensions for subtraction." << endl;
        return Mat1;
    }
    // Getting past the if statement means both matrices have the same dimensions.
    else {
        Matrix subMat;
        int diff = 0;
        int i, j;
        for (i = 0; i < Mat1.m; i++) {
            for (j = 0; j < Mat1.n; j++) {
                diff = Mat1.data[i][j] - Mat2.data[i][j];
                subMat.data[i][j] = diff;
            }
        }
        return subMat;
    }
    
}

// Overloaded unary minus operator
Matrix operator -(const Matrix& Mat1){
    
    Matrix minusMat;
    int i, j;
    for (i = 0; i < Mat1.m ; i++) {
        for (j = 0; j < Mat1.n ; j++)
            minusMat.data[i][j] = (Mat1.data[i][j])*(-1);
    }
    return minusMat;
    
}

// Overloaded equality operator
bool operator ==(const Matrix& Mat1, const Matrix& Mat2){
    
    // Check that the dimensions of both matrices are equal first.
    if( ( Mat1.m != Mat2.m ) || ( Mat1.n != Mat2.n ) ){
        return false;
    }
    // Now check if every element is the same.
    else {
        int i, j;
        for (i = 0; i < Mat1.m; i++) {
            for (j = 0; j < Mat1.n; j++) {
                if (Mat1.data[i][j] != Mat2.data[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }
    
}

int main() {
    
    typedef Matrix* MatPtr;
    
    Matrix A;
    cout << A;

    ifstream fin;
    fin.open("file.txt");
    fin.close(); // We are now done with our input file, so close it.
    
//    // Z is zero matrix.
//    Matrix Z;
//    cout << "\nMatrix Z (Zero Matrix):" << endl;
//    cout << Z;
//    
//    // D is 2 diagonal, E is 1 diagonal.
//    Matrix D(2);
//    cout << "Matrix D (Diagonal-2 Matrix):" << endl;
//    cout << D;
//    Matrix E(1);
//    cout << "Matrix E (Diagonal-1 Matrix):" << endl;
//    cout << E;
//    
//    // Initialize A to zero first, then input stuff.
//    Matrix A;
//    fin >> A; // input works.
//    cout << "Matrix A (from file):" << endl;
//    cout << A;
//    
//    // B is a copy of A.
//    Matrix B(A);
//    cout << "Matrix B (copy of A):" << endl;
//    cout << B;
//    
//    // Check that B is indeed a copy of A.
//    if( B==A )
//        cout << "Yes, B==A." << endl; // equality works.
//    
//    // Check that our arithmetic operators work.
//    cout << "\nResult from A+D:" << endl;
//    cout << A+D; // add works.
//    cout << "Result from A-D:" << endl;
//    cout << A-D; // binary - works
//    cout << "Result from A*D:" << endl;
//    cout << A*D; // mult works.
//    
//    // Check that the equality operator works.
//    if( A-B==Z )
//        cout << "Yes, A-B==Z." << endl;   
//    if( -A==Z-A )
//        cout << "\nYes, –A==Z-A." << endl;
//    if( A+B==A*D )
//        cout << "\nYes, A+B==A*D." << endl;
//    if( A*E==A )
//        cout << "\nYes, A*E==A." << endl;
//    
//    cout << "\nThe determinant of D is: " << D.det() << endl;
//    cout << "The determinant of E is: " << E.det() << endl;
//    
//    // Initialize C to zero first, then input stuff.
//    Matrix C;
//    fin >> C; // input works.
//    cout << "\nMatrix C (from file):" << endl;
//    cout << C;
    
//    
//    if( (A*C).det()==A.det()*C.det() )
//        cout << "Yes, det(A*C) is the same as det(A)*det(C)." << endl;   
    
    return 0;
    
}

