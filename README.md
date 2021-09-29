STM32F103C8T6+CCS811二氧化碳浓度检测     
    
 
本文使用的IIC协议 参考ltengy大佬的文章，需要的可以去github看看
https://ltengy.github.io/2018/08/04/ccs811%E8%B0%83%E8%AF%95%E6%8C%87%E5%8D%97/

主程序根据CCS811手册来进行编写      
记得在APP_Start后在测量模式选择时一定要加延时，要不然很可能出错，不能正确选择模式。    
这个模块感觉很奇怪所以得多试试，如果出错,得重置模块 然后再进入重复上述操作    
根据情况不同有可能开始测量的速度有所不同，我有时候开始有测量值快，有时候有测量值慢，这主要还是看MODE1什么时候设置成功。   

测试：     
     
![image](https://github.com/dbwaax/STM32F103C8T6-CCS811-/blob/main/images/200523sxi079lp79i750i2.png)      
![image](https://github.com/dbwaax/STM32F103C8T6-CCS811-/blob/main/images/2.png)
