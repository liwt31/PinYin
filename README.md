# PinYin Input Method

A simple pinyin input method program written in C++ that converts pinyin to corresponding Chinese character based on dual character model.

---
### How to use
1. `cd bin` into `./bin/` directory. Run the program in other directories may cause failure when loading necessary resources, specifically `./data/pinyin_map.txt` and `./data/dissipation_map.txt`.
2. For windows, use `pinyin.exe INPUT_FILE_PATH OUTPUT_FILE_PATH` to run the program. For Linux, use `pinyin  INPUT_FILE_PATH OUTPUT_FILE_PATH`. Sample `INPUT_FILE_PATH` is `../data/input.txt`. The output file is GBK encoded.
3. To compile the code, use `cmake` then `make`.

### Content of directories
* `./bin/` - Compiled exe (64 bit). `main.exe` for windows and `main` for Linux.
* `./data/` - Preprocessed data for the pinyin program to run as well as input/output files.
* `./src/` - Source files. A directory containing preprocess (frequency extraction) python script is also given. 

### Program design
* The algorithm is based on dual character model. The tendency of not converting a pinyin string to a certain Chinese character, dissipation, is determined by the conditional probability for the character to appear after the previous character. The whole pinyin sentence is thus converted to the most probable character sentence by dynamic programing. 
* The program is divided to 4 modules.
	* Main module that deals with interface with upper caller and prints debug and benchmark infomation， including `main.cpp`.
	* IO module that reads resources and write output, including `io.cpp` and `io.h`.
	* Dynamic programing module that searches the most probable sentence, including `search.cpp` and `search.h`.
	* A character candidate class that acts as the node in the dynamic programing, including `CharCandidate.cpp` and `CharCandidate.h`. The class not only records current character, but also records all previous characters in a string.
* The probabilities of character combinations are extracted from sina news corpus using Python scripts. The dissipations are basically the log of the probabilities. See the **About preprocessing** part for more details.

### Implementation of the search module
* Character candidates are listed in a STL priority queue. The candidate that represents the currently most probable combination of characters is kept at the top of the heap.
* First an empty character noted as "##" is pushed into the queue representing the start of the sentence. The overall dissipation of the starting character candidates is defined as 0. 
* At every iteration:
	*  pop the top of the heap and examine the position of the last character and try to find the next pinyin string according to the position.
	* If there's no next pinyin string at all, then the most probable character sentence is found.
	* If a pinyin string is found, find all corresponding Chinese characters using resources in `pinyin_map.txt`. Then for all new characters find the dissipation of its combination with the previous character and add the dissipation to the previous overall dissipation.
		* The dissipations of the starting character "##" and other single characters are determined by the appearance frequencies of these single characters.
	* Push all the new character candidates in the priority queue. 
	* Go to next iteration.
* The dissipatiosn of character combinations are recorded in a STL unordered map from `dissipation_map.txt`. For  combinations not in the map, the result is obtained from the combination of "##" and the last character. If the last character is not recorded even in combination with "##", which means it does not appear in the whole sina news corpus, the largest dissipation in the unordered map is assigned to it.

### About preprocessing
* Preprocessing is Python based for higher development effeciency and is divided into 2 parts. The first part is a character and word counter for the sina news corpus. And the second is to find the corresponding dissipation from the counting result.
* The character and word count are first converted to conditional probabilities, and then modulated by character frequency to avoid dissipation for unseen words. The constant for the modulation is `alpha` which will be discussed in **Analysis of parameters**. The negative log of the probabilities are taken as the dissipations. "##" acting as a abstract symbol is also added to the mapping to provide a unified interface for dissipations of single characters.
* The dissipations are powered by a parameter `beta` to provide better performance. See **Analysis of parameters** for details.

### Output examples
* Typicall inputs are:
```
qing hua da xue ji suan ji xi
wo shang xue qu le
jin tian hui jia bi jiao wan
liang hui zai bei jing zhao kai
zi qiang bu xi hou de zai wu
ren gong zhi neng
ji qi xue xi
shu ju wa jue
jian chi quan xin quan yi wei ren min fu wu
liang hui zai bei jing zhao kai qing hua da xue ji suan ji xi
```
* And the outputs are:
```
清 华 大 学 计 算 机 系
我 上 学 去 了
今 天 回 家 比 较 完
两 会 在 北 京 召 开
子 将 不 息 后 的 灾 物
人 工 智 能
机 器 学 习
数 据 挖 掘
坚 持 全 新 权 益 为 人 民 服 务
两 会 在 北 京 召 开 清 华 大 学 计 算 机 系
```
* We can see from the examples that the program generally performs well and the bottleneck should be the dual character model used. For example, clearly the "correct" output for "bi jiao wan" should be "比较晚" rather than "比较完". But in my word count it seems "较完" is more common than "较晚" due to three character combinations such as "较完备", "较完善" and so on. A triple character model should effectively solve the problem. Another problem in the ouputs is the performance for idioms like "zi qiang bu xi" or "quan xin quan yi", which could be solved using models with more characters. 

### Analysis of parameters

* As only few md grammers support LaTeX equations, no numerical expression is used here.
* `alpha` and `beta` are the two main parameters tested in the project, and they are all in the preprocess module. `alpha` is the percentage of the conditional probability contribution to the dissipation of a character combination and the remaining part is the frequency of the single character. `beta` is used to modulate the distribution of dissipations by powering them.
* Smaller `alpha` or `beta` prefers low frequency words and *vice versa*.
* I carried out multiple tests with different parameters to find total number of character candidates searched to produce the Chinese character sentence output. Note that all tested outputs are basically the same. The results are shown in the following table with rows for `alpha` and columns for `beta`. "-" notes overflow.

|alpha\beta|0.8|1.4|2.0|2.6|
|:--|:--|:--|:--|:--|
|0.80|-|846821|47477|9820|
|0.85|-|650875|37813|8104|
|0.90|-|473952|28656|6237|
|0.95|1111335|304853|19711|4272|

* Greater `alpha` and `beta` could reduce the computation cost with 3 orders of magnetude without much compromise in accuracy.

