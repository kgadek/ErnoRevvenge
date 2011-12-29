#ifndef __MATRIXCUBE_HPP
#define __MATRIXCUBE_HPP

#include <iostream>
#include <utility>
#include <cstring>
#include <algorithm>

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
void setColour(std::ostream &out, int c=0) {
	out << "[" << c << "m";
}

/**
 * Template klasy opisującej kostkę DxDxD.
 *
 * W tej klasie trzymam się poniższej notacji
 *   - i -- numer ściany
 *   - j -- wiersz
 *   - k -- kolumna
 *
 * @tparam D Rozmiar kostki
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
	 * @see operator<<(std::ostream&, matrixCube&);
	 */
	int matrix[6][D][D];

	/**
	 * Funkcja identycznościowa (obrót współrzędnych o zero stopni).
	 */
	std::pair<int,int> id(int x, int y) {
		return std::pair<int,int>(x,y);
	}
	
	/**
	 * Obrót współrzędnych zgodnie ze wskazówkami zegara
	 */
	std::pair<int,int> cw(int x, int y) { 
		return std::pair<int,int>(y,D-1-x);
	}
	
	/**
	 * Obrót współrzędnych przeciwnie do ruchu wskazówek zegara
	 */
	std::pair<int,int> ccw(int x, int y) {
		return std::pair<int,int>(D-1-y,x);
	}

	/**
	 * Obrót współrzędnych o 180 stopni
	 */
	std::pair<int,int> ud(int x, int y) { 
		return std::pair<int,int>(D-1-x,D-1-y);
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
	 * Wyświetlanie kostki na ekranie.
	 *
	 * Funkcja specyficzna dla powłoki Linuksa.
	 * @param out Strumień wyjściowy.
	 * @param m Kostka do wyświetlenia.
	 * @see geColour
	 * @see setColour
	 */
	template <int R>
	friend std::ostream& operator<<(std::ostream &out, matrixCube<R> &m);

	/**
	 * Wczytywanie kostki.
	 *
	 * @param in Strumień wejściowy.
	 * @param m Kostka, do której zostaną wczytane dane.
	 */
	template <int R>
	friend std::istream& operator>>(std::istream &in, matrixCube<R> &m);

	/**
	 * Stwierdza, czy kostka jest ułożona.
	 *
	 * @warning Nie jest sprawdzane, czy kostka jest poprawna. Co istotne - nie
	 *  są sprawdzane wartości pól. Jeśli wartość nie będzie z zakresu [0,5] to
	 *  w najlepszym wypadku program się wywali.
	 * @see isCorrect
	 * @return True \f$\iff\f$ kostka jest ułożona.
	 */
	bool isSolved() {
		int i,j,k;
		bool colours[5] = {0,0,0,0,0};
		int currColour;
		for(i=0; i<5; ++i) {
			currColour = matrix[i][0][0];
			if(colours[currColour])
				return false;
			for(j=0; j<D; ++j)
				for(k=0; k<D; ++k)
					if(matrix[i][j][k] != currColour)
						return false;
		}
		return true;
	}

	/**
	 * Sprawdzenie poprawności kostki.
	 *
	 * Nie jest jednak sprawdzane, czy taki układ jest możliwy do wygenerowania --
	 * poniższy test sprawdza jedynie, czy
	 *   - elementy mają dobre kolory (zakres [0,5])
	 *   - ilość elementów w danym kolorze jest właściwa
	 * 
	 * @returns True \f$\iff\f$ kostka jest poprawna.
	 */
	bool isCorrect() {
		int i,j,k,jj,kk;
		const int cSize = (D+1)/2;
		int colours[5][cSize][cSize];
		int val;
		for(i = 0; i<5; ++i)
			for(j = 0; j<cSize; ++j)
				for(k = 0; k<cSize; ++k)
					colours[i][j][k] = 0;
		for(i = 0; i<5; ++i) {
			for(j = 0; j<D; ++j) {
				for(k = 0; k<D; ++k) {
					val = matrix[i][j][k];
					if(val < 0 || val > 5)
						return false;
					jj = (j >= cSize) ? (cSize-j+1) : j;
					kk = (k >= cSize) ? (cSize-k+1) : k;
					if( ++(colours[i][jj][kk]) > 4) {
						std::cout << "!" << i << j << k << jj << kk << colours[i][jj][kk] << "\n";
						return false;
					}
				}
			}
		}
		return true;
	}
};

template <int D>
std::istream& operator>>(std::istream &in, matrixCube<D> &m) {
	int i,j,k;

	for(i=0; i<6; ++i)
		for(j=0; j<D; ++j)
			for(k=0; k<D; ++k)
				std::cin >> m.matrix[i][j][k];

	return in;
}

template <int D>
std::ostream& operator<<(std::ostream &out, matrixCube<D> &m) {
	static std::string dashes = std::string(D, '-') + "+";
	static std::string spaces = std::string(D, ' ');
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
	std::pair<int,int> (matrixCube::*convCoords[][4])(int,int) = { 
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

	std::copy((int*)matrix, (int*)matrix+6*D*D, (int*)nMatrix);

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
			std::pair<int,int> (matrixCube::*conv)(int,int) = convCoords[rFace][cnt];
			std::pair<int,int> (matrixCube::*conv2)(int,int) = convCoords[rFace][(cnt+1)%4];
			for(j=0; j<rDepth; ++j) {
				for(k=0; k<D; ++k) {
					std::pair<int,int> c = (this->*conv)(j,k);
					int &jj = c.first;
					int &kk = c.second;
					std::pair<int,int> c2 = (this->*conv2)(j,k);
					int &jj2 = c2.first;
					int &kk2 = c2.second;
					nMatrix[i2][jj2][kk2] = matrix[i][jj][kk];
				}
			}
		}

		std::copy((int*)nMatrix, (int*)nMatrix+6*D*D, (int*)matrix);
	}

	return *this;
}


#endif
