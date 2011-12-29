#include <iostream>
#include <cstdio>
#include <algorithm>
#include <utility>

using namespace std;

/**
 * Proste odwzorowanie: numer ściany -> kolor.
 * BTW: wolę British English więc colOUr a nie colOr ;)
 * @param i Numer koloru.
 * @return Kod ASCII zmiany ustawień konsoli.
 * @see http://ascii-table.com/ansi-escape-sequences.php
 */
int getColour(int i) {
	switch(i) {
		case 0: return 41;
		case 1: return 44;
		case 2: return 47;
		case 3: return 42;
		case 4: return 43;
		case 5: return 45;
		default: return 0;
	}
}

/**
 * Ustawia kolor c na wyjściu out
 * @param out Wyjście.
 * @param c Kolor.
 * @see http://ascii-table.com/ansi-escape-sequences.php
 */
void setColour(ostream &out, int c=0) {
	out << "[" << c << "m";
}

/**
 * Template klasy opisującej kostkę DxDxD.
 *
 * W tej klasie trzymam się poniższej notacji
 *   - i -- numer ściany
 *   - j -- wiersz
 *   - k -- kolumna
 */
template <int D>
struct matrixCube {
	/**
	 * Macierz opisująca ustawienie kostki.
	 *
	 * Numeracja: numer ściany, wiersz, kolumna.
	 *
	 * Numer ściany:
	 *   - czerwona = 0
	 *   - niebieska = 1
	 *   - biała = 2
	 *   - zielona = 3
	 *   - żółta = 4
	 *   - pomarańczowa = 5
	 *
	 * Wiersz i kolumna zgodny z reprezentacją kostki w 2D.
	 *
	 * @see operator<<(ostream&, matrixCube&);
	 */
	int matrix[6][D][D];

	/**
	 * Funkcja identycznościowa (obrót współrzędnych o zero stopni).
	 */
	pair<int,int> id(int x, int y) {
		return pair<int,int>(x,y);
	}
	
	/**
	 * Obrót współrzędnych zgodnie ze wskazówkami zegara
	 */
	pair<int,int> cw(int x, int y) { 
		return pair<int,int>(y,D-1-x);
	}
	
	/**
	 * Obrót współrzędnych przeciwnie do ruchu wskazówek zegara
	 */
	pair<int,int> ccw(int x, int y) {
		return pair<int,int>(D-1-y,x);
	}

	/**
	 * Obrót współrzędnych o 180 stopni
	 */
	pair<int,int> ud(int x, int y) { 
		return pair<int,int>(D-1-x,D-1-y);
	}

	/**
	 * Domyślny konstruktor.
	 *
	 * Ustawia (ułożoną) kostkę w położeniu domyślnym, tj. białym do przodu, czerwonym do góry
	 */
	matrixCube() {
		int i,j,k;
		for(i=0; i<6; ++i)
			for(j=0;j<D;++j)
				for(k=0;k<D;++k)
					matrix[i][j][k] = i;
	}

	/**
	 * Modyfikacja układu kostki.
	 *
	 * Obraca daną ścianę rFace przeciwnie do ruchu wskazówek zegara
	 * rTimes razy, obracając jej rDepth poziomów.
	 *
	 * @param rFace Numer ściany, wokół której obracamy.
	 * @param rDepth Ilość poziomów, które obracamy.
	 * @param rTimes Ilość obrotów. Obroty zgodnie ze wskazówkami zegara. By obrócić w prawo podajemy rTimes=-1.
	 */
	matrixCube& rotate(int rFace, int rDepth, int rTimes);

	/**
	 * Dostęp do wewnętrznej macierzy.
	 */
	inline int& operator()(int i, int j, int k) {
		return matrix[i][j][k];
	}

	template <int R>
	friend ostream& operator<<(ostream&, matrixCube<R>&);
};

int main() {
	matrixCube<4> test00;
	matrixCube<4> test01;
	matrixCube<4> test02;
	matrixCube<4> test03;
	matrixCube<4> test04;
	matrixCube<4> test05;
	matrixCube<4> test105;
	matrixCube<4> test1;
	matrixCube<3> testA;
	cout << "test00:\n" << test00.rotate(0, 1, 1);
	cout << "test01:\n" << test01.rotate(1, 1, 1);
	cout << "test02:\n" << test02.rotate(2, 1, 1);
	cout << "test03:\n" << test03.rotate(3, 1, 1);
	cout << "test04:\n" << test04.rotate(4, 1, 1);
	cout << "test05:\n" << test05.rotate(5, 1, 1);
	cout << "test05:\n" << test05.rotate(5, 1, 1);
	cout << "test105:\n";
	cout << test105.rotate(1, 1, 1);
	cout << test105.rotate(0,2,1);
	cout << test105.rotate(5,1,1);
	cout << "test1:\n" << test1.rotate(1,4,1);
	cout << "testA:\n" << testA;
	testA(0,0,0) = 0;
	cout << testA << testA.rotate(2,1,1);
	return 0;
}

/**
 * Wyświetlanie kostki na ekranie.
 *
 * Funkcja specyficzna dla powłoki Linuksa.
 * @param out Potok wyjściowy.
 * @param m Kostka do wyświetlenia.
 * @see geColour
 * @see setColour
 */
template <int D>
ostream& operator<<(ostream &out, matrixCube<D> &m) {
	static string dashes = string(D, '-') + "+";
	static string spaces = string(D, ' ');
	int i,j,k;

	setColour(out); // default
	out << spaces << " +" << dashes << "\n";
	for(j=0; j<D; ++j) {
		out << spaces << " |";
		for(k=0; k<D; ++k) {
			setColour(out, getColour(m.matrix[0][j][k]));
			out << m.matrix[0][j][k];
		}
		out << "[0m"; // default
		out << "|\n";
	}
	out << "+" << dashes << dashes << dashes << dashes << "\n";
	for(j=0; j<D;++j) {
		for(i=1; i<5; ++i) {
			out << "|";
			for(k=0; k<D; ++k) {
				setColour(out, getColour(m.matrix[i][j][k]));
				out << m.matrix[i][j][k];
			}
			setColour(out); // default
		}
		out << "|\n";
	}
	setColour(out); // default
	out << "+" << dashes << dashes << dashes << dashes << "\n";
	for(j=0; j<D; ++j) {
		out << spaces << " |";
		for(k=0; k<D; ++k) {
			setColour(out, getColour(m.matrix[5][j][k]));
			out << m.matrix[5][j][k];
		}
		setColour(out); // default
		out << "|\n";
	}
	out << spaces << " +" << dashes << "\n";
	return out;
}


template <int D>
matrixCube<D>& matrixCube<D>::rotate(int rFace, int rDepth, int rTimes) {

	/*
	 * Tablica obrotów.
	 * Zawiera informacje o przejściu krawędzi na krawędź przy danym obrocie.
	 *
	 * Przykładowo:
	 * Obrót względem ściany 2 oznacza, że część ściany 0 przechodzi w 1, część 1
	 * w 5, część 5 w 3, oraz część 3 w 0.
	 */
	int rots[][4] = {
		{4,1,2,3},
		{0,4,5,2},
		{0,1,5,3},
		{0,2,5,4},
		{0,3,5,1},
		{2,1,4,3}
	};

	/*
	 * Ten potworek tutaj to dwuwymiarowa tablica wskaźników na funkcje
	 * składowe matrixCube przyjmujące dwa inty i zwracające pare intów.
	 */
	pair<int,int> (matrixCube::*convCoords[][4])(int,int) = { 
		{ &matrixCube::id,  &matrixCube::id,  &matrixCube::id,  &matrixCube::id  },
		{ &matrixCube::ccw, &matrixCube::cw,  &matrixCube::ccw, &matrixCube::ccw },
		{ &matrixCube::ud,  &matrixCube::cw,  &matrixCube::id,  &matrixCube::ccw },
		{ &matrixCube::cw,  &matrixCube::cw,  &matrixCube::cw,  &matrixCube::ccw },
		{ &matrixCube::id,  &matrixCube::cw,  &matrixCube::ud,  &matrixCube::ccw },
		{ &matrixCube::ud,  &matrixCube::ud,  &matrixCube::ud,  &matrixCube::ud  },
	};

	int nMatrix[6][D][D];
	int *nMatrixH = &nMatrix[0][0][0];
	int *matrixH = &matrix[0][0][0];
	int i,j,k;
	int cnt,i2;

	rTimes %= 4;
	if(!rTimes)
		return *this;

	copy((int*)matrix, (int*)matrix+6*D*D, (int*)nMatrix);

	// obrót przedniej ściany
	for(cnt = 0; cnt<rTimes; ++cnt)
		for(j=0; j<D; ++j)
			for(k=0; k<D; ++k)
				nMatrix[rFace][D-1-k][j] = matrix[rFace][j][k];

	// obrót pozostałych ścian
	while(rTimes-- > 0) {
		for(cnt = 0; cnt<4; ++cnt) {
			i = rots[rFace][cnt];
			i2 = rots[rFace][(cnt+1)%4];
			pair<int,int> (matrixCube::*conv)(int,int) = convCoords[rFace][cnt];
			pair<int,int> (matrixCube::*conv2)(int,int) = convCoords[rFace][(cnt+1)%4];
			for(j=0; j<rDepth; ++j) {
				for(k=0; k<D; ++k) {
					pair<int,int> c = (this->*conv)(j,k);
					int &jj = c.first;
					int &kk = c.second;
					pair<int,int> c2 = (this->*conv2)(j,k);
					int &jj2 = c2.first;
					int &kk2 = c2.second;
					nMatrix[i2][jj2][kk2] = matrix[i][jj][kk];
				}
			}
		}

		copy((int*)nMatrix, (int*)nMatrix+6*D*D, (int*)matrix);
	}

	return *this;
}

