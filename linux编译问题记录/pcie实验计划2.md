可以，压缩为 **21 天全职冲刺版**，每天约 8 小时，共约 160–175 小时。目标仍是达到“能读、能算、能实验、能初步调试”，不是简单背概念。

Gen1 不会被删掉 注释 1；它仍作为理解 8b/10b、Ordered Set、初始建链和后续代际演进的基线，只压缩重复内容。

## 每天固定节奏

- 30 分钟：闭卷复习前一天。
- 2 小时：教程或讲义。
- 1.5 小时：按主题查规范。
- 3 小时：解包、Linux、仿真或 FPGA 实验。
- 1 小时：整理当日图表和结论。

第 7、14 天以验收和查漏补缺为主，可缩短到 6 小时。

## 第 1 周：架构、配置空间、TLP

| 天数    | 学习内容                                                     | 当日实验与验收                                               |
| ------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| 第 1 天 | RC、Root Port、Endpoint、Switch；Link/Lane；三层协议栈；Gen1/2/3 速率与编码 | 画完整拓扑和三层图；计算三代 x1/x4/x8/x16 单向带宽           |
| 第 2 天 | BDF；256 B 配置空间与 4 KB 扩展空间；Type 0/1 Header；标准与扩展 Capability | 用 `lspci -xxxx` 标出 Vendor ID、Device ID、Class Code、Header Type 和 Capability 链 |
| 第 3 天 | 枚举；Bridge 总线号；BAR sizing；32/64 位、Prefetchable BAR；地址路由和桥窗口 | 写枚举伪代码；从真实设备还原 BAR 和上游桥窗口                |
| 第 4 天 | MPS、MRRS、LnkCap/LnkSta；MSI/MSI-X；Bus Master；DMA、IOMMU、AER、设备电源状态 | 制作 Capability 速查表；画出 CPU MMIO、Endpoint DMA 和 MSI 三条路径 |
| 第 5 天 | Memory、I/O、Configuration、Message；Posted、Non-Posted、Completion；3DW/4DW Header | 手解 10 个 TLP，标出 Fmt/Type、Length、Requester ID、Tag、地址、Byte Enable |
| 第 6 天 | Cpl/CplD；Completion Status、Byte Count、Lower Address；MPS、MRRS、RCB、4 KB 边界、顺序规则 | 画一个 512 B Memory Read 被拆成多个 CplD 的时序图            |
| 第 7 天 | PH/PD、NPH/NPD、CplH/CplD 六类 Credit；第一阶段考试          | 根据 Credit 判断 TLP 能否发送；配置空间和 TLP 测试达到 80%   |

第一周结束时必须能解释：

- `LnkCap` 是能力，`LnkSta` 是实际状态。
- Memory Write 通常是 Posted。
- Memory Read 使用 Tag 与 Completion 匹配。
- MSI/MSI-X 在线路上本质是 Memory Write。
- MPS、MRRS、链路速度、链路宽度是不同概念。

## 第 2 周：可靠性、Gen1/2/3 PHY 和 LTSSM

| 天数     | 学习内容                                                     | 当日实验与验收                                               |
| -------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| 第 8 天  | Sequence Number、LCRC、ACK/NAK、Replay Buffer、Replay Timer；ECRC；AER 基础 | 画出正常发送、NAK 重发、ACK 丢失后三种流程                   |
| 第 9 天  | 高速串行基础；Gen1 2.5 GT/s；8b/10b、Running Disparity、K-code、Comma、Scrambling、TS1/TS2、SKP、FTS | 解释编码和扰码的区别；说明每种 Ordered Set 的作用            |
| 第 10 天 | Gen2 5 GT/s；仍为 8b/10b；去加重、速率能力交换、Recovery.Speed、向 Gen1 回退 | 对比 Gen1/Gen2 的 UI、带宽、编码和训练过程                   |
| 第 11 天 | LTSSM 正常路径：Detect→Polling→Configuration→L0；接收端检测、位锁定、Lane Number、宽度协商 | 闭卷画主路径，并给每个状态写出一句进入和退出条件             |
| 第 12 天 | Recovery.RcvrLock/RcvrCfg/Speed/Idle；速率切换、降速、降宽；L0s/L1/L2；Hot Reset、FLR、PERST# | 分析“Polling 卡死”“x8 变 x4”“频繁进 Recovery”三个案例        |
| 第 13 天 | Gen3 8 GT/s；128b/130b、Sync Header、Block Lock、Framing、SKP/EIEOS、Lane Deskew | 解释 Gen3 为什么接近 Gen2 的两倍；计算准确比例约 1.97 倍     |
| 第 14 天 | Gen3 三抽头 FIR；P0–P10；FS/LF；CTLE/DFE；Equalization Phase 0–3；第二阶段考试 | 闭卷画 Gen1 建链并升到 Gen3 的状态路径；准确说明 Phase 2/3 方向 |

Gen3 四阶段需要掌握：

- Phase 0：交换并应用初始 Preset，向 8 GT/s 切换。
- Phase 1：在初始设置下建立通信，交换 FS/LF 等能力。
- Phase 2：Endpoint RX 评估并调节 Root Port TX。
- Phase 3：Root Port RX 评估并调节 Endpoint TX。

第二周结束时必须分清：

- ACK DLLP 不等于 Completion TLP。
- Credit 防止缓冲溢出；ACK/NAK 保证单跳可靠性。
- Replay Timer 不等于 Completion Timeout。
- LTSSM 的 Configuration 不等于操作系统配置空间。
- L0 表示物理链路建立，不代表操作系统已经枚举并加载驱动。
- Preset 编号不是质量排名。

## 第 3 周：实战、性能、调试和最终项目

| 天数     | 学习内容                                                     | 当日实验与验收                                               |
| -------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| 第 15 天 | 建立实验基线                                                 | 选一个非关键 Endpoint，记录拓扑、Root Port、BAR、MPS/MRRS、MSI-X、AER、最大/当前速度与宽度 |
| 第 16 天 | BAR、MMIO、内存屏障、MSI/MSI-X、复位                         | 使用 QEMU 或 FPGA Endpoint 完成寄存器读写和中断；没有硬件则分析现成配置空间 |
| 第 17 天 | Bus Master DMA、DMA 地址、IOMMU、Scatter-Gather、队列、Outstanding Tag | 完成双向 DMA 或事务模型测试；至少校验 1 GiB 累计数据         |
| 第 18 天 | 性能模型；MPS、MRRS、Tag、Credit、Completion、队列深度、中断合并 | 比较小块/大块、低/高队列深度；解释理论带宽与实测差距         |
| 第 19 天 | 故障树；AER；链路消失、降速、降宽、Recovery 循环、Completion Timeout、驱动失败 | 对五个未知故障写出“事实→所属层→假设→最小验证动作”            |
| 第 20 天 | 综合项目                                                     | 完成“PCIe Gen1–Gen3 链路剖析与故障诊断”报告                  |
| 第 21 天 | 3 小时闭卷考试、4 小时实操、错题修复                         | 全部核心项目达到下面的验收标准                               |

## 第 15 天建议使用的只读命令

```
lspci -Dnn
lspci -tv
sudo lspci -s 0000:03:00.0 -nnvvv
sudo lspci -s 0000:03:00.0 -xxxx
cat /sys/bus/pci/devices/0000:03:00.0/current_link_speed
cat /sys/bus/pci/devices/0000:03:00.0/current_link_width
cat /sys/bus/pci/devices/0000:03:00.0/max_link_speed
cat /sys/bus/pci/devices/0000:03:00.0/max_link_width
journalctl -k -g 'PCIe|AER'
```

不要在系统盘、主显卡或唯一网卡上进行：

- `setpci` 写寄存器。
- 强制降速或 Retrain。
- FLR、驱动解绑。
- AER 错误注入。
- 原始磁盘写性能测试。

## 最终项目要求

报告至少包含：

1. 真实 PCIe 拓扑。
2. Root Port 和 Endpoint 配置空间解析。
3. BAR、MPS、MRRS、MSI/MSI-X、AER。
4. 三代理论带宽及当前实际链路状态。
5. CPU 写 BAR 的 TLP/DLLP 路径。
6. DMA Read 的 Request、Completion、ACK 路径。
7. Detect 到 Gen3 L0 的 LTSSM 路径。
8. Gen3 Equalization 四阶段。
9. 理论带宽和实测性能差距。
10. 三个故障案例及证据化排查过程。

## 第 21 天通过标准

- 配置空间解析正确率不低于 85%。
- 10 个 TLP 至少正确解出 8 个。
- 能计算任意 Gen1/2/3、任意 Lane 宽度的理论带宽。
- 能完整讲清 Credit、ACK/NAK、Replay、Completion。
- 能闭卷画出 LTSSM 和 Gen3 Equalization。
- 能定位设备不出现、Gen3 降 Gen2、x8 降 x4、Recovery 循环和低吞吐问题。
- 不混淆 ACK/Completion、LCRC/ECRC、Replay Timer/Completion Timeout、能力/当前状态。

为了守住 21 天，只建立索引、不深入 SR-IOV、ATS、PRI、PASID、ACS、多 VC、TPH、AtomicOp、L1 Substates 和完整合规测试。