# 2014-2학기 알고리즘 수업때 짯던 허프만 코딩. 책은 알고리즘 기초 4판(도경구역)

---

먼저 데이터 파일과 헤드 파일을 부득이하게 나누게 되었음을 말씀드립니다.

이 두파일이 하나의 zip 파일로 합쳐서 압축 해제를 하려고 해보았지만 바이너리 입출력에서

어딘지 모르게 바이트 배열이 흐트러지는 것 같습니다. 대신에 데이터 파일(바이너리) .hufcomp

헤드 파일을 .huftable 로 나누어 별도로 저장하게 되었습니다.

해제 할 때 .huftable 과 .hufcomp 파일을 입력 스트림으로 읽습니다.

압축해제한 파일이름은 OriginalFile 입니다.

다음은 프로그램에 대한 개략적인 설명입니다.
(끝의 한 두문자 정도가 깨집니다. 나머지는 정상적으로 압축 - 해제가 되는 걸로 확인하였습니다.)

메인 함수에의 what_to_process는 유닉스에서 제공하는 옵션을 처리하는 라이브러리를 사용하여
-c와 -d에 대해 각각 압축, 해제를 하도록 유도합니다.
압축을 할 경우, HuffmanCompress 함수를 호출하고,
해제를 할 경우, HuffmanDecompress 함수를 호출합니다.


압축은 아스키 파일을 읽고 사용된 문자와 빈도수를 구하고 이를 바탕으로 허프만 코드 알고리즘을 사용하여 인코딩을 합니다.
테이블을 위한 자료 구조는 STL map을 사용하였습니다.
map<unsigned char, unsigned int> HuffmanMapTable;

인코딩 하기 전에 바이너리 트리를 구축하는 과정이 필요한 데, 이는 BuildHuffmanTree라는 함수로 구현 하였습니다.

BuildHuffmanSymbolEncoding라는 함수는 만들어진 바이너리 트리를 순회 하며 리프 노드에 있는 문자에 대한 인코딩을
수행하고 문자와 이진수로 구성된 문자열 상수로 된 테이블을 만듭니다. 
map<unsigned char, string > HuffmanSymbolEncoding;

이후 아스키 파일을 다시 처음부터 문자 하나씩 읽으면서 만들어놓은 자료구조를 바탕으로 압축을 진행합니다.

이 과정에서 BitString이라는 스트링 변수를 사용하였는데, 한 문자에 해당하는 이진수 문자열 변수를 먼저 한 곳에 모아둡니다.

그리고 나서 WriteObjectFile이라는 함수로 이진수 문자열 변수를 바이너리 모드로 출력합니다.

다음엔 필요한 정보를 출력하고 zip 파일을 생성합니다. 

이 프로그램에선 기본적으로 .hufcomp(압축된 바이너리 파일) 와 .huftable(해제 할때, 필요한 테이블 구축을 위한 빈도수 파일)이라는 두 가지 파일을 먼저 출력하고 난 이후 둘을 합친 .zip 파일을 생성합니다.

압축 해제는 HuffmanDecompress함수에서 수행합니다. 

압축 때 진행된 .hufcomp와 .huftable를 읽습니다.

이로부터 트리를 구축하고 바이너리 모드로 압축 파일을 읽습니다. 

.hufcomp 바이너리 모드로 읽는 코드는 다음과 같습니다. 
    unsigned char *buf = new unsigned char[ObjSize];
    InObjectFile.read((char *)buf, ObjSize); InObjectFile.close();

buf의 각 원소에 대해 연산을 할 때, 8비트를 2진 문자열로 바꿔주는 getBitString이란 함수를 사용하여

unsigned char 변수 하나를 2진수로 된 문자열로 바꾸어 BitString 이라는 곳에 몽땅 집어 넣습니다.

이후에  BuildHuffmanTree, BuildHuffmanSymbolEncoding로 트리를 만들고 문자 인코딩을 거친다음,

BitString을 인수로 하는 DecodeHuffmanCode함수는 BitString을 갖고 트리를 뒤져가며 매칭 하는 문자를 찾고

이들을 모아 아스키 파일(OriginalFile)을 출력하게 되는데, 이 파일이 결과적으로 해제된 파일입니다.

