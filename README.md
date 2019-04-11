此source code 是我2年前剛學DDS (Data Distribution Service)時，想抽換DDS成ZMQ時的POC

最初為了讓zeromq實作的通訊功能能reuse而建立一套新的library

過程中發現，此library的設計與OpenSplice C99 API (ADLINK Prismtech DDS產品)極為相似

因此甚至最後會猜測並沿用OpenSplice C99 API的設計概念

----------------------------------------------------------
以下為2年前的筆記，轉到google blog上

#內文提到的是VortexLite 是原先OpenSplice 的分支輕量產品

#不過近幾年VortexLite的C99 API已經被納入OpenSplice了

https://karlkwchen.blogspot.com/2019/04/zeromq-libraryvortex-lite-library.html
