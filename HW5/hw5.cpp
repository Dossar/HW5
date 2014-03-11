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
    Matrix(int rows, int columns); // For our arithmetic operations
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
    
    int i, j;
    
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
    for (i = 0; i < m ; i++ )
        data[i] = new int[n]; // Allocate for each column.
    
    // File the matrix with all zeros.
    for (i = 0; i < m ; i++) {
        for (j = 0; j < n ; j++)
            data[i][j] = 0;
    }
   
}

// Constructor with parameters. Creates diagonal matrix.
Matrix::Matrix(int diagvalue) {
    
    int i, j;
    
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
    for (i = 0; i < m ; i++ )
        data[i] = new int[n];
    
    // Make the diagonal matrix after inputting square dimensions.
    for (i = 0; i < m; i++) {
        // Initialize entire row to zero first, then insert number into diagonal.
        for (j = 0; j < n; j++)
            data[i][j] = 0;
        j = 0; // Reinitialize j to zero so we can get the correct column in the diagonal
        j += i;
        data[i][j] = diagvalue;
    }

}

// Arithmetic Matrix constructor
Matrix::Matrix(int rows, int columns) {
    
    int i, j;
    m = rows;
    n = columns;
    
    // Make dynamic matrix. A matrix is an array of arrays.
    data = new int*[m];
    for (i = 0; i < m ; i++ )
        data[i] = new int[n];
    
    // File the matrix with all zeros.
    for (i = 0; i < m ; i++) {
        for (j = 0; j < n ; j++)
            data[i][j] = 0;
    }

}

// Copy constructor
Matrix::Matrix(const Matrix& M)
{
    
    int i, j;
    m = M.m;
    n = M.n;
    
    // Make dynamic matrix. A matrix is an array of arrays.
    data = new int*[m]; // Allocate for each row.
    for (i = 0; i < m ; i++ )
        data[i] = new int[n]; // Allocate for each column.
    
    // Copy elements from referenced matrix.
    for (i = 0; i < m ; i++) {
        for (j = 0; j < n ; j++)
            data[i][j] = M.data[i][j];
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
        // i for Mat1 rows, j for Mat2 columns, k for Mat2 rows and Mat1 columns.
        int i, j, k;
        
        // Product matrix dimensions is first matrix rows by second matrix columns.
        Matrix multMat( Mat1.m , Mat2.n );
        
        int sum = 0;
        
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
        Matrix addMat( Mat1.m , Mat1.n );
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
        Matrix subMat( Mat1.m , Mat1.n );
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
    
    Matrix minusMat( Mat1.m , Mat1.n );
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
    
    cout << "Make Zero Matrix. Make sure dimensions are 3x2." << endl;
    Matrix Z;
    cout << "[Matrix Z] Zero Matrix:" << endl;
    cout << Z;

    ifstream fin;
    fin.open("file.txt");
    
    cout << "[Matrix A] input from file. Make sure dimensions are 3x2." << endl;
    Matrix A;
    fin >> A;
    cout << A;
    
    Matrix B(A);
    cout << "[Matrix B] copy of A:" << endl;
    cout << B;
    
    if( B==A )
        cout << "Yes, A and B are the same.\n" << endl;
    
    cout << "[Matrix C] input from file. Make sure dimensions are 3x2." << endl;
    Matrix C;
    fin >> C;
    cout << C;
    fin.close(); // We are now done with our input file, so close it.
    
    cout << "Make diagonal matrix of 1s. Make sure dimensions are 2x2." << endl;
    Matrix E(1);
    cout << "[Matrix E] Matrix of 1-diagonal:" << endl;
    cout << E;
    
    cout << "Make diagonal matrix of 2s. Make sure dimensions are 2x2." << endl;
    Matrix D(2);
    cout << "[Matrix D] Matrix of 2-diagonal:" << endl;
    cout << D;
    
    if( A==B && !(A==C) )
        cout << "A == B, but A != C." << endl;
    
//    if( !(A==C) )
//        cout << "A is not equal to C." << endl;
    
    if( A-B==Z )
        cout << "A-B is the Zero Matrix." << endl;
    
    if( (Z-A)==(-A) )
        cout << "–A == Z-A is true." << endl;
    
    if( A+B==A*D )
        cout << "A+B == A*D is true." << endl;
    
    if( A*E==A )
        cout << "A*E == A is true." << endl; 
    
    return 0;
    
}

