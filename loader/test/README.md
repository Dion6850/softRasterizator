# loader模块测试

```bash
cmake -B build
cmake --build build
cd build/Debug
loader_test.exe --files ../../test.obj another.obj
```

# 测试结果：

```bash
D:\iGAME\softResterizator\loader\test\build\Debug>loader_test.exe --files ../../test.obj another.obj
[内存监控] 启动时内存: 3788 KB
加载不存在文件: 失败
[内存监控] 加载不存在文件后内存: 4136 KB
加载../../test.obj: 成功
[内存监控] 加载../../test.obj 前: 4136 KB, 后: 4704 KB, 增量: 568 KB
顶点数: 3301
三角形数: 6598
第一个顶点: (0.688061, -0.491338, -0.0132902)
第一个三角形: (0.563338, -0.42551, -0.0774944) - (0.578094, -0.452874, -0.035497) - (0.551737, -0.469801, -0.0777987)
加载another.obj: 失败
[内存监控] 加载another.obj 前: 4780 KB, 后: 4780 KB, 增量: 0 KB
[内存监控] 程序结束时内存: 4780 KB
```

| 模型顶点数 | 模型片元数 | 内存消耗 |
| - | ---- | ---- |
| 3301 |  6598    |  568KB    |
