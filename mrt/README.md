# taipeiMRT_shortestpath
台北捷運系統-尋找最短路徑


### 功能:

*	尋找mrt兩站之間最短路徑並計算出花費金額
*	若有需要避開的站名則予以避開

### 邏輯:
```=
以dynamic programming 方式，搜尋最短路徑。
遞迴呼叫find_shortest_path ，輸入起始站和終點站，先判斷兩站是否相同，若相同，則直接回傳path。
若不同，則先尋找起始站在哪幾條顏色線上，得知在哪些顏色線上後。分別找出他的相鄰站點，
再用他的相鄰站點遞迴呼叫find_shortest_path，回傳時會先比較出最短路徑再回傳path。
```


```=
每次呼叫回傳:
Step 1.Source到某相鄰解節點的count + min(所有可以從某相鄰節點到destination的path count)
Step 2. 再從source 到所有相鄰節點中篩選出path 最短者回傳
```
