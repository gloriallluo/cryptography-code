# Enigma 破解实验

计86 罗境佳 2018013469



**文件结构说明**

`src` 目录：Enigma 实验的源代码

`practice` 目录：小练习的源代码



## 实验内容

-   实现一个 Enigma 密码机。
-   选择采用图灵法进行破解。

### 运行方法

**在 ./src 目录下运行下面的命令。**

简洁版本，仅输出明密文以及破解出的明文：

```shell
make run
```

调试版本，输出算出的循环圈、Rotor Setting 以及 Plugboard 的检测结果等等：

```shell
make debug
```

###实验结果

采用已知明密文攻击。

```shell
# Thy huntress' name that my full life doth sway.
plain:  THYHUNTRESSNAMETHATMYFULLLIFEDOTHSWAY
secret: DUEAAVITHLTSWRMQLEQSFKWOYQBPGYYQLWBBB
```

**运行大约 1 分钟出现结果**，`decode` 即为所解密的明文。

```shell
secret: DUEAAVITHLTSWRMQLEQSFKWOYQBPGYYQLWBBB
decode: THYHUGTRESSGAMETHATMYFULLLIFCDOTHSWAY
plain:  THYHUNTRESSNAMETHATMYFULLLIFEDOTHSWAY
```

解出的密码机的设置：

```shell
# Plugboard
#          A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
Detected:  I U R F E D ? X A ? ? L T ? W ? Y C S M B ? O H Q ? 
Origin:    I U R F E D N X A Z K L T G W P Y C S M B V O H Q J
# Rotor Setting
Check passed!
First Rotor 2, Pos H.
Second Rotor 1, Pos B.
Third Rotor 4, Pos Y.
```

解码过程中，共确定 19 个字母在接线板中的设置，其中包括 8 对接线板设置（分别为 A & I, B & U, C & R, D & F, H & X, M & T, O & W, Q & Y）。而由于循环圈个数限制，G & N, J & Z 这两对没有被发现。

可以看出，由于有的接线板交换字母对没有确定，最后解密出的 `decode` 字符串与明文 `plain` 仍有一些不同。在解密中尚未确定的字母有 7 个，由于知道已经破解出 8 对交换字母对，可以知道剩下有 2 对位于这 7 个字母中，尚未确定的排布有 $\frac{A_7^4}{2! \times 2^2} = 105$ 种，可以枚举一下肉眼鉴别出合理的排布。



## 具体实现

### Enigma密码机的实现

```c++
int plugBoard[N]; 	// 插线板
int rotors[5][N]; 	// 转轮（正向）
int rotorsR[5][N];	// 转轮（反向）
int offset[3];	  	// 3 个转轮的位置
int reflector[N];	// 反射器
int chosen[3];		// 3 个被选中的转轮
```

一个字母的加密过程就是：plugBoard => update offset => rotors * 3 => reflector => rotorsR => plugBoard。

其中 `offset[]` 的进位的实现较为麻烦，大致逻辑如下：

```c++
carry = (offset[0] = (offset[0] + 1) % 26) == carryFlag[chosen[0]];
if (!carry) return;
// 继续 offset[1] 的进位，offset[2] 以此类推
```

具体可见 `enigma.h` 以及 `enigma.cpp` 文件。

### 发现循环圈

函数 `dfsCircle` 用于发现循环圈，循环圈用一个这样的数据结构存储：

```c++
using Circle = std::vector<char>;
```

一个循环圈的信息包括首字母以及一系列 offset，也就是说在一套 Rotor Setting （包括 3 个转轮以及初始的 offset）下，该首字母经过这一系列的变换后仍是自身。

最终发现的循环圈大致有这些：

```shell
New Circle: A: 17 8 1 4 
New Circle: A: 17 8 3 
New Circle: A: 17 14 13 7 0 29 2 8 1 4 
New Circle: A: 17 14 13 7 0 29 2 8 3 
New Circle: A: 17 14 19 9 23 30 2 8 1 4 
New Circle: A: 17 14 19 9 23 30 2 8 3 
New Circle: A: 17 14 19 9 24 2 8 1 4 
New Circle: A: 17 14 19 9 24 2 8 3 
New Circle: A: 17 14 19 10 0 29 2 8 1 4 
New Circle: A: 17 14 19 10 0 29 2 8 3 
New Circle: D: 29 2 8 1 4 17 14 13 7 0 
New Circle: D: 29 2 8 1 4 17 14 19 10 0 
New Circle: D: 29 2 8 3 17 14 13 7 0 
... # 写不下了，可以运行 `make debug` 看完整的输出
```

如果是正确的 Rotor Setting，一定会满足发现的所有循环圈，于是利用这些循环圈去排除错误的 Rotor Setting 以及确定 Plugboard Setting。

### 破解算法

`main` 函数中破解的部分的伪码如下：

```python
for 26 * 26 * 26 种 offset:
	for 5 * 4 * 3 种 rotors:
		# 确定一种 Rotor Setting
		dfsPlug() # 验证 Rotor Setting 以及确定插线板
```

确定 Plugboard Setting 的 `dfsPlug` 函数遍历所有的循环圈，对于一个字母 `initial`，如果发现它经过某个循环圈的一套变化之后变回了它自己，则可以推断 `initial` 与该循环圈的首字母 `head` 发生了交换，于是在这个假设上继续搜索。



# 简答题

### 1.5

源代码：`practice/shift.py`

移位密码的加密方式是每个字母被字母表中之后的第 K 个字母替代，枚举 K，得到如下结果：

```shell
BEEAKFYDJXUQYHYJIQRYHTYJIQFBQDUYJIIKFUHCQD
CFFBLGZEKYVRZIZKJRSZIUZKJRGCREVZKJJLGVIDRE
DGGCMHAFLZWSAJALKSTAJVALKSHDSFWALKKMHWJESF
EHHDNIBGMAXTBKBMLTUBKWBMLTIETGXBMLLNIXKFTG
FIIEOJCHNBYUCLCNMUVCLXCNMUJFUHYCNMMOJYLGUH
GJJFPKDIOCZVDMDONVWDMYDONVKGVIZDONNPKZMHVI
HKKGQLEJPDAWENEPOWXENZEPOWLHWJAEPOOQLANIWJ
ILLHRMFKQEBXFOFQPXYFOAFQPXMIXKBFQPPRMBOJXK
JMMISNGLRFCYGPGRQYZGPBGRQYNJYLCGRQQSNCPKYL
KNNJTOHMSGDZHQHSRZAHQCHSRZOKZMDHSRRTODQLZM
LOOKUPINTHEAIRITSABIRDITSAPLANEITSSUPERMAN
MPPLVQJOUIFBJSJUTBCJSEJUTBQMBOFJUTTVQFSNBO
NQQMWRKPVJGCKTKVUCDKTFKVUCRNCPGKVUUWRGTOCP
ORRNXSLQWKHDLULWVDELUGLWVDSODQHLWVVXSHUPDQ
PSSOYTMRXLIEMVMXWEFMVHMXWETPERIMXWWYTIVQER
QTTPZUNSYMJFNWNYXFGNWINYXFUQFSJNYXXZUJWRFS
RUUQAVOTZNKGOXOZYGHOXJOZYGVRGTKOZYYAVKXSGT
SVVRBWPUAOLHPYPAZHIPYKPAZHWSHULPAZZBWLYTHU
TWWSCXQVBPMIQZQBAIJQZLQBAIXTIVMQBAACXMZUIV
UXXTDYRWCQNJRARCBJKRAMRCBJYUJWNRCBBDYNAVJW
VYYUEZSXDROKSBSDCKLSBNSDCKZVKXOSDCCEZOBWKX
WZZVFATYESPLTCTEDLMTCOTEDLAWLYPTEDDFAPCXLY
XAAWGBUZFTQMUDUFEMNUDPUFEMBXMZQUFEEGBQDYMZ
YBBXHCVAGURNVEVGFNOVEQVGFNCYNARVGFFHCREZNA
ZCCYIDWBHVSOWFWHGOPWFRWHGODZOBSWHGGIDSFAOB
ADDZJEXCIWTPXGXIHPQXGSXIHPEAPCTXIHHJETGBPC
```

其中较为合理的是这一句：

```shell
# Look up in the air. It's a bird. It's a plane. It's superman.
LOOKUPINTHEAIRITSABIRDITSAPLANEITSSUPERMAN
```



### 1.16

**(a)**  $\pi^{-1}$ 如下所示

```
1	2	3	4	5	6	7	8
2	4	6	1	8	3	5	7
```

**(b)** 解密过程如下

```
ETEGENLMDNTNEOORDAHATECOESAHLRMI
ETEGENLM|DNTNEOOR|DAHATECO|ESAHLRMI
GENTLEME|NDONOTRE|ADEACHOT|HERSMAIL
GENTLEMENDONOTREADEACHOTHERSMAIL
Gentlemen do not read each other's mail.
```



### 1.21

**a**

词频统计如下：

```shell
A 0.018 B 0.000 C 0.136 D 0.029 E 0.044 F 0.033 G 0.088 
H 0.018 I 0.055 J 0.026 K 0.066 L 0.026 M 0.018 N 0.048 
O 0.037 P 0.022 Q 0.004 R 0.000 S 0.074 T 0.000 U 0.051 
V 0.000 W 0.018 X 0.026 Y 0.055 Z 0.048
```

根据出现频率最高的几个字母推测有如下的代换规律：

-   密文 C - 明文 E
-   密文 I - 明文 D
-   密文 K - 明文 S
-   密文 F - 明文 W

尝试代换后如下：

```
EMGLOSUDeGDNeUSWYSwHNSweYsDPUMLWGYdeOXYSdPJesQPsUGs
MGOLdeGdNeGAesSNdSAeYsZSesXEeJesSHYSXeGOdDPsZeNsSHd
eGdWYGssGsGOLDSdLsGOdUSdGLEDSPWZUGwZeeNDGYYSwUSZeNX
EOJNeGYEOWEUPXEZGAeGNwGLsNSAedGOdYesXeJUedUZewZeeND
GYYSwEUEsUZeSOewZeeNedAeZEJNeSHwZEJZEGMXeYHeJUMGsUeY
```

观察这段文字，wzeen 出现次数较多，推测为 wheel，因此可以推断出密文 Z - 明文 H、密文 N - 明文 L；dayytw 也出现较多，但是暂时猜不出来。密文 E 出现的频率中等，猜测其明文可能是 D、I、H、N、O、S 中的一个；而它又出现在段首，所以猜测可能性最大的是 I。

接下来统计双字频率，因为已知 Z 对应 H，UZ 的频率较高，推测 U 的明文是 T，代换后如下：

```
iMGLOStDeGDletSWYSwHlSweYsDPtMLWGYdeOXYSdPJesQPstGs
MGOLdeGdleGAesSldSAeYshSesXieJesSHYSXeGOdDPshelsSHd
eGdWYGssGsGOLDSdLsGOdtSdGLiDSPWhtGwheelDGYYSwtShelX
iOJleGYiOWitPXihGAeGlwGLslSAedGOdYesXeJtedthewheelD
GYYSwitistheSOewheeledAehiJleSHwhiJhiGMXeYHeJtMGsteY
```

再对 **dgyys**w 进行猜测，再结合前面的词是 wheel，而 wheel barrow 是手推车的意思，因此有密文 D - 明文 B、密文 G - 明文 A、密文 Y - 明文 R，密文 S 明文 - 明文 O。且密文 M 跟在明文 I 后面，猜测句子开头可能是 I'm a 或者 I may 或者 I can，结合字母 M 的频率推测 M 变换后仍然是 M。代换后如下，并且尝试分词：

```
i maL Oot be able to Wrow Hlowers bPt mLWardeOXrodP
JesQPst as maOL dead leaAes old oAer shoes XieJesoH
roXeaOdbPshelsoH dead Wrass as aOLbodLs aOd todaL i 
boPWht a wheel barrow to helX iO JleariOW it PX i 
haAe alwaLs loAedaOdresXeJted the wheel barrow it is 
the oOe wheeled AehiJle oH whiJh i am XerHeJt master
```

猜测有 maL - may，Oot - not，Wlow - grow，Hlowers - flowers，bPt - but，leaAes - leaves，boPWht - bought，helX - help，iO - in，AehiJle - vehicle。得到最终的答案：

```
i may not be able to grow flowers, but my garden 
produces just as many dead leaves, old overshoes, 
pieces of rope and bushels of dead grass as anybody's.
and today i bought a wheel barrow to help in clearing 
it up. i have always loved and respected the wheel 
barrow. it is the one wheeled vehicle of which i am 
perfect master.
```



**b**

采用 Kasiski 测试，先搜索相同的长度为 3 密文片段，寻找距离的公约数。发现字符串 HJV 和 KFT 分别出现了 5 次和 3 次，打印它们的距离：

```shell
HJV:  18 138 54 12  # index: 107 125 263 317 329
KFT:  18 156 		# index: 79 97 253
```

密钥长度最多为 6，也有可能是 3、2。进行重合指数分析：

```shell
0.0385 0.0471 # 2
0.0559 0,0481 0.0483 # 3
0.0627 0.0838 0.0494 0.0649 0.0429 0.0734 # 6
```

发现还是 6 更接近自然语言。根据英文 3 字母的词频，推测 HJV 对应 THE，密钥的位移为 `[24, 9, _, _, _ 12]`，然后发现 KFT 对应的竟然也是 THE，于是有 `[24, 9, 2, 11, _, 12]`。

计算明文：

```
ILEAKNEDHOPTOCALVULATEMHEAMONNTOFPTPERNEXDEDFOK
AROOMPHENIWTSATSCAOOLYONMULTIILYTHELQUAREYOOTAG
XOFTHEPALLSBRTHECUUICCONMENTSOYTHEFLHORANDVEILI
NZCOMBIGEDANDWOUBLEBTYOUTAENALLHWHALFMHETOTTLFO
ROIENINGLSUCHALWINDOPSANDDHORSTHXNYOUAELOWTHXOT
HERAALFFOKMATCHBNGTHEIATTERGTHENYHUDOUBEETHEWAO
LETHBNGAGABNTOGIOEAMARZINOFEKRORANWTHENYHUORDEK
THEPAIER
```

可以推测出位移是 `[24, 9, 2, 11, 7, 12]`，密钥为 `YJCLHM`。

最终的明文为：

```
i learned how to calculate the amount of paper 
needed for a room when i was at school. you 
multiply the square footage of the walls by 
the cubic contents of the floor and ceiling 
combined and double it. you then allow half the 
total for openings such as windows and doors. 
then you allow the other half for matching the 
pattern then you double the whole thing again 
to give a margin of error and then you order 
the paper.
```



**c**

首先进行字母频率统计，猜测有密文 C - 明文 E，密文 B - 明文 T，于是有以下的等式：

```shell
4 = (2 * a + b) mod 26  # C -> E
19 = (1 * a + b) mod 26 # B -> T
```

其中 a、b 均小于 26，a 与 26 互素。可以得出 `a = 11, b = 8`。

于是将密文进行解密，有：

```shell
ocanadaterredenosaieuxtonfrontestceintdefleuronsg
lorieuxcartonbrassaitporterlepeeilsaitporterlacro
ixtonhistoireestuneepopeedesplusbrillantsexploits
ettavaleurdefitrempeeprotegeransfoyersetnosdroits
```



**d**

可能是普通的代换密码、Virginia 密码、仿射密码，统计字频发现每个字母的频率都差不多，比较可能是 Virginia 密码。然后像第 2 题一样寻找相同的 3 个字母之间的距离，猜测密钥长度。

```shell
AVF:  132 # 38 170
GES:  180 # 77 257
MMA:  120 # 89 209
MAS:  120 # 90 210 
WDV:  174 # 164 338
DVY:  174 # 165 339
...
```

这些距离的最大公约数为 6，同样猜测密钥长度为 6、3、2，得出 6 比较接近自然语言。

最终类似地，得知密钥是 `[7, 19, 22, 12, 9, 2]` 得到明文：

```
i grew up among slow talkers men in particular who 
dropped words a few at a time like beans in a hill. 
and when i got to minneapolis where people took a 
lake wobegon comma to mean the end of a story i 
couldn't speak a whole sentence in company and was 
considered not too briaht. so i enrolled in a speech 
couqse taught by orvilles and the founder of 
reflexive relaxology, a self hypnotic technique that 
enabled a person to speak up to three hundred words 
per minute.
```



### 1.26

**(a)**

将密文按列排列在一个 m * n 的矩阵中，按行读取即可获得明文。

**(b)**

遍历各种可能的 m 和 n，最后选择出符合自然语言的：

```
marymaryquitecontraryhowdoesyourgardengrow
```

