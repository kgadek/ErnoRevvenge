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
 * Klasa opisująca kostkę.
 *
 * W tej klasie trzymam się poniższej notacji
 *   - i - numer ściany
 *   - j - wiersz
 *   - k - kolumna
 */
class matrixCube {
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
	int matrix[6][4][4];
	int front;	/**< Numer ściany, na którą patrzymy */
	int head;	/**< Numer ściany, która jest na górze */

	pair<int,int> id(int x, int y) { /**< Funkcja identycznościowa (obrót współrzędnych o zero stopni). */
		return pair<int,int>(x,y);
	}
	
	pair<int,int> cw(int x, int y) { /**< Obrót współrzędnych zgodnie ze wskazówkami zegara. */
		return pair<int,int>(y,3-x);
	}
	
	pair<int,int> ccw(int x, int y) { /**< Obrót współrzędnych przeciwnie do ruchu wskazówek zegara. */
		return pair<int,int>(3-y,x);
	}
	
	pair<int,int> ud(int x, int y) { /**< Obrót współrzędnych o 180 stopni. */
		return pair<int,int>(3-x,3-y);
	}

public:
	/**
	 * Domyślny konstruktor.
	 *
	 * Ustawia (ułożoną) kostkę w położeniu domyślnym, tj. białym do przodu, czerwonym do góry
	 */
	matrixCube() : front(2), head(0) {
		int i,j,k;
		for(i=0; i<6; ++i)
			for(j=0;j<4;++j)
				for(k=0;k<4;++k)
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
	void rotate(int rFace, int rDepth, int rTimes);
	friend ostream& operator<<(ostream&, matrixCube&);
};

int main() {
	matrixCube test;
	cout << "bla\n";
	cout << test;
	cout << "ble\n";
	cout << endl;
	test.rotate(2, 1, 1);
	cout << test;
	cout << "bli\n";
	cout << endl;
	return 0;
}

/**
 * Wyświetlanie kostki na ekranie.
 *
 * Funkcja specyficzna dla powłoki Linuksa.
 * @params out Potok wyjściowy.
 * @params m Kostka do wyświetlenia.
 * @see geColour
 * @see setColour
 */
ostream& operator<<(ostream &out, matrixCube &m) {
	/*
	 *      0----+
	 *      |    |
	 *      |    |
	 *      |    |
	 *      |    |
	 * 1----2----3----4----+
	 * |    |    |    |    |
	 * |    |    |    |    |
	 * |    |    |    |    |
	 * |    |    |    |    |
	 * +----5----+----+----+
	 *      |    |
	 *      |    |
	 *      |    |
	 *      |    |
	 *      +----+
	 */
	int i,j,k;
	setColour(out); // default
	out << "     +----+\n";
	for(j=0; j<4; ++j) {
		out << "     |";
		for(k=0; k<4; ++k) {
			setColour(out, getColour(m.matrix[0][j][k]));
			out << m.matrix[0][j][k];
		}
		out << "[0m"; // default
		out << "|\n";
	}
	out << "+----+----+----+----+\n";
	for(j=0; j<4;++j) {
		for(i=1; i<5; ++i) {
			out << "|";
			for(k=0; k<4; ++k) {
				setColour(out, getColour(m.matrix[i][j][k]));
				out << m.matrix[i][j][k];
			}
			setColour(out); // default
		}
		out << "|\n";
	}
	setColour(out); // default
	out << "+----+----+----+----+\n";
	for(j=0; j<4; ++j) {
		out << "     |";
		for(k=0; k<4; ++k) {
			setColour(out, getColour(m.matrix[5][j][k]));
			out << m.matrix[5][j][k];
		}
		setColour(out); // default
		out << "|\n";
	}
	out << "     +----+\n";
}


void matrixCube::rotate(int rFace, int rDepth, int rTimes) {

	/**
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

	/**
	 * Ten potworek tutaj to dwuwymiarowa tablica wskaźników na funkcje
	 * składowe matrixCube przyjmujące dwa inty i zwracające pare intów.
	 */
	pair<int,int> (matrixCube::*convCoords[][4])(int,int) = { 
		{ &matrixCube::id,  &matrixCube::id,  &matrixCube::id,  &matrixCube::id  },
		{ &matrixCube::cw,  &matrixCube::ccw, &matrixCube::cw,  &matrixCube::cw  },
		{ &matrixCube::ud,  &matrixCube::cw,  &matrixCube::id,  &matrixCube::ccw },
		{ &matrixCube::ccw, &matrixCube::ccw, &matrixCube::ccw, &matrixCube::cw  },
		{ &matrixCube::id,  &matrixCube::ccw, &matrixCube::ud,  &matrixCube::cw  },
		{ &matrixCube::ud,  &matrixCube::ud,  &matrixCube::ud,  &matrixCube::ud  },
	};

	int nMatrix[6][4][4];
	int *nMatrixH = &nMatrix[0][0][0];
	int *matrixH = &matrix[0][0][0];
	int i,j,k;
	int cnt,i2;

	rTimes %= 4;
	if(!rTimes)
		return;

	for(i=0;i<6;++i)
		for(j=0;j<4;++j)
			for(k=0;k<4;++k)
				nMatrix[i][j][k] = matrix[i][j][k];

	// obrót przedniej ściany
	for(cnt = 0; cnt<rTimes; ++cnt)
		for(j=0; j<4; ++j)
			for(k=0; k<4; ++k)
				nMatrix[rFace][3-k][j] = matrix[rFace][j][k];

	// obrót pozostałych ścian
	while(rTimes-- > 0) {
		for(cnt = 0; cnt<4; ++cnt) { // dla każdej ściany z otoczenia
			i = rots[rFace][cnt];
			i2 = rots[rFace][(cnt+1)%4];
			cout << "Ściana " << i << " w ścianę " << i2 << "\n";
			pair<int,int> (matrixCube::*conv)(int,int) = convCoords[rFace][cnt];
			pair<int,int> (matrixCube::*conv2)(int,int) = convCoords[rFace][(cnt+1)%4];
			for(j=0; j<rDepth; ++j) {
				for(k=0; k<4; ++k) {
					pair<int,int> c = (this->*conv)(j,k);
					int &jj = c.first;
					int &kk = c.second;
					pair<int,int> c2 = (this->*conv2)(j,k);
					int &jj2 = c2.first;
					int &kk2 = c2.second;
					cout << "\t[" << i2 << "][" << jj2 << "][" << kk2 << "] <-- [" << i << "][" << jj << "][" << kk << "]\n";
					nMatrix[i2][jj2][kk2] = matrix[i][jj][kk];
				}
			}
		}

		for(i=0;i<6;++i)
			for(j=0;j<4;++j)
				for(k=0;k<4;++k)
					matrix[i][j][k] = nMatrix[i][j][k];
	}
}

