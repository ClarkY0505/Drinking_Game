# Game Concept: 命牌 — 牌即一生，得必有失

*Created: 2026-06-09*
*Status: Draft*

---

## Elevator Pitch

> 这是一款回合制卡牌叙事 RPG，你在连锁遭遇中押注手牌、操控骰子概率、以连招引爆命运。每一次出牌前的决策、每一次掷骰时的紧张，以及每一次"得必有失"的舍取——共同塑造了一个只属于你的角色传记。

---

## Core Identity

| Aspect | Detail |
| ------ | ------ |
| **Genre** | 回合制卡牌叙事 RPG |
| **Platform** | 移动端（iOS / Android），PC 可用于原型开发 |
| **Target Audience** | 喜欢卡牌构筑与叙事选择的玩家 |
| **Player Count** | 单人 |
| **Session Length** | 10-30 分钟 |
| **Monetization** | 待定 |
| **Estimated Scope** | 中型（6-7 个月，独立开发） |
| **Comparable Titles** | Slay the Spire（卡牌构筑）、Dicey Dungeons（骰子驱动）、Reigns（选择与舍取） |

---

## Core Fantasy

> 你不是旁观者——你就是这副牌本身。每一张牌是你生命中的一个关键时刻：一段羁绊、一次牺牲、一个誓言。你通过连招与骰子操控去对抗命运，同时每一次获得都伴随着失去。当游戏结束时，你拥有的不是一副"最强构筑"——而是一部独一无二的角色传记。

---

## Unique Hook

> 类似 Slay the Spire 的卡牌战斗，**AND ALSO** 先出牌后掷骰——你押注命运，再用特殊卡牌操控骰子概率。同时你的牌组不是"战力面板"，而是角色一生的传记——每一次获得一张牌，必须舍弃另一张牌。

---

## Player Experience Analysis (MDA Framework)

### Target Aesthetics (What the player FEELS)

| Aesthetic | Priority | How We Deliver It |
| ---- | ---- | ---- |
| **Fantasy** (make-believe, role-playing) | 1 | 牌组即角色——每一次拿牌与弃牌都在书写角色传记 |
| **Challenge** (obstacle course, mastery) | 2 | 连招规划 + 概率操控 + 舍取决策 = 三层博弈 |
| **Narrative** (drama, story arc) | 3 | 得必有失的舍取节点推动剧情分支 |
| **Expression** (self-expression, creativity) | 4 | 每副牌组独一无二，是你的身份签名 |
| **Discovery** (exploration, secrets) | 5 | 卡牌连锁效果、骰子概率调控的组合发现 |
| **Sensation** | 6 | 连招引爆 + 骰子动画 + 卡牌获取/舍弃视觉效果 |
| **Submission** (relaxation) | N/A | 此游戏不是放松型体验 |
| **Fellowship** (social) | N/A | 单人游戏，无社交要素 |

### Key Dynamics (Emergent player behaviors)
- 玩家将反复试验"先押哪张牌后押哪张牌"的顺序以求最大化连锁收益
- 玩家会在得必有失的舍取节点上犹豫不决——因为舍弃的不只是数值，而是一个故事时刻
- 玩家会建立起"概率自信"——不是靠运气，而是靠操控骰子的牌把概率压到自己这边
- 每次跑完一局后，玩家会想查看"我这副牌里承载了哪些记忆"

### Core Mechanics (Systems we build)
1. **连锁出牌 + 一次性掷骰检定**：先连 2-5 张牌形成连招链，然后掷骰一次性引爆全部结果
2. **概率操控卡牌**：部分卡牌可以在掷骰前修改骰子的概率分布（如"下一次 d6 检定 +1"、"重掷 3 以下的结果"）
3. **得必有失的舍取系统**：获取任何新卡牌 → 必须舍弃一张现有卡牌
4. **线性 + 分支叙事引擎**：数据驱动的对话/事件/分支脚本
5. **骰子系统**：多面骰（d4/d6/d8/d10/d12），视觉动画表现

---

## Player Motivation Profile

### Primary Psychological Needs Served

| Need | How This Game Satisfies It | Strength |
| ---- | ---- | ---- |
| **Autonomy** | 每回合出牌顺序自由选择，舍取节点完全自决，牌组身份由你定义 | **Core** |
| **Competence** | 读懂局面→规划连招→操控概率→押注成功，整套过程的掌握感 | **Core** |
| **Relatedness** | 与角色（自己创造的角色身份）建立深层情感连接 | **Supporting** |

### Player Type Appeal (Bartle Taxonomy)

- [x] **Achievers** — 通过连招连锁实现高光时刻，收集故事结局
- [x] **Explorers** — 发掘卡牌连锁组合、骰子操控的数学美感
- [ ] **Socializers** — 单人游戏
- [ ] **Killers/Competitors** — 无 PvP 要素

### Flow State Design

- **Onboarding**: 首局前 10 分钟线性引导——固定卡牌组、固定的得必有失节点、简化的骰子检定（仅 d6），让玩家先理解"连招→掷骰→结果"三步节奏
- **Difficulty scaling**: 遭遇难度曲线由卡牌检定所需的骰值阈值递增——不是数值碾压，而是对连招规划的要求越来越高
- **Feedback clarity**: 连招结算动画——逐牌展示"检定通过 / 检定失败 / 连锁触发"，玩家一眼看懂胜负在哪里
- **Recovery from failure**: 一局失败后角色牌组重置、故事从头开始——但每次跑都会产生不同的故事（roguelike 结构的天然重玩性）

---

## Core Loop

### 30 秒循环
审视遭遇局面 → 从手牌选择 2-5 张牌连成连招链 → 一次性掷骰 → 连招结算动画（逐牌检定，连锁触发涟漪效果）

### 5 分钟循环
一个连锁遭遇——包含若干轮连招 + 骰子操控博弈，以遭遇结果（战胜/说服/逃脱）结束。这是"再来一局"的心理引力点。

### 一局（30-60 分钟）
若干连锁遭遇 → 2-3 个"得必有失"舍取节点 → 牌组持续演变 → 最终结局

### 长线走向
每次运行生成不同的牌组身份——不同选择、不同舍取，产生不同的角色传记。玩家有动力重跑看不同结局。

### Retention Hooks
- **好奇心**："如果我舍弃这张牌而不是那张牌，故事会怎样？"
- **投入感**：玩家对"自己一手造就的牌组身份"产生情感依附
- **掌握感**：逐步精通连招链规划和概率操控的数学美感

---

## Game Pillars

### Pillar 1: 连招即叙事
每套连招都是一段可以被记住的故事——"我那次叠了四张牌，骰子全过，一发逆转"。连招不只是数值叠加，而是产生意外事件、触发连锁效果、改写当前局面。

*Design test*: 如果在这张牌的效果和二选一的叙事节点之间犹豫 → 选这张牌。

### Pillar 2: 概率是武器，不是天气
骰子不是被动承受的运气——你可以通过卡牌操控概率分布、保底、重掷。赢家不靠脸，靠用牌把概率押到自己这边。

*Design test*: 如果某个骰子检定没有任何卡牌能影响它 → 这个检定需要被移除或增加干预手段。

### Pillar 3: 牌即一生，得必有失
卡牌是角色生命中的关键时刻。获取一张新牌不仅需要叙事选择，还需要舍弃另一张牌——牌组的每一次变化都意味着角色失去了一些东西、得到了一些东西。

*Design test*: 如果获取一张牌不需要任何舍弃（失去另一张牌/放弃一个选项/承担一个代价）→ 这条叙事分支需要设计一个代价。

### Pillar 4: 短期玩法，长线记忆
每个遭遇 5-10 分钟可完成，随时可以停下。但长线的叙事和牌组演变让人反复回来。

*Design test*: 如果犹豫应该优先做遭遇的深度还是广度 → 先做更多遭遇（保证新鲜感），再做单个遭遇的复杂度。

### Anti-Pillars (What This Game Is NOT)

- **NOT 最优套路**: 不做"找到一张核心牌堆几十张复制品"的构筑。牌组上限有限，没有两张完全相同的牌。连招的价值在于搭配，不是堆砌。（保护 Pillar 1）
- **NOT 预设构筑**: 开局不能精挑细选牌组。牌由故事给你，不是由你在菜单里配好。（保护 Pillar 3）
- **NOT 纯数值碾压**: 不存在"攻击力翻倍""血量无限"式的膨胀。难度来自获取关键时刻需要的卡牌，而不是需要更强的卡牌。（保护 Pillar 2）

---

## Inspiration and References

| Reference | What We Take From It | What We Do Differently | Why It Matters |
| ---- | ---- | ---- | ---- |
| Slay the Spire | 卡牌构筑 + 回合制遭遇的紧凑循环 | 先出牌后掷骰 + 得必有失的舍取系统 | 验证了单人卡牌游戏的巨大市场 |
| Dicey Dungeons | 骰子作为核心资源驱动玩法 | 骰子是检定机制，不是资源池——且你可以操控概率 | 验证了骰子 + 卡牌的趣味性 |
| 暗黑破坏神 3/4 | 技能连锁的快感 + 刷装备的满足感 | 卡牌连锁替代技能组合 + 牌组演变替代装备掉落 | 验证了组合多巴胺的驱动力 |
| 仙剑奇侠传 | 情感叙事 + 角色成长 | 叙事直接嵌入牌组机制（得必有失） | 验证了中文 RPG 玩家对情感叙事的深度共鸣 |
| Reigns | 抉择 + 舍取的二元性 | 卡牌连锁 + 骰子检定替代简单滑屏 | 验证了"舍取"是强有力的叙事驱动 |

**Non-game inspirations**: 塔罗牌的命运感、禅宗的"得即失、失即得"、每张卡牌像一页日记一样承载一段记忆

---

## Target Player Profile

| Attribute | Detail |
| ---- | ---- |
| **Age range** | 18-35 |
| **Gaming experience** | 中核——有卡牌游戏或 RPG 经验 |
| **Time availability** | 碎片时间 10-30 分钟（移动端天然匹配） |
| **Platform preference** | 手机——通勤、睡前、午休 |
| **Current games they play** | Slay the Spire、崩坏：星穹铁道、Monster Train、暗黑 Immortal |
| **What they're looking for** | 有深度但不冗长的策略体验 + 能留下情感记忆的叙事 |
| **What would turn them away** | 无脑氪金、纯靠运气的随机、没有情感重量的刷子游戏 |

---

## Technical Considerations

| Consideration | Assessment |
| ---- | ---- |
| **Engine** | SDL2 + C++ 自研轻量引擎（已有框架代码） |
| **Key Technical Challenges** | UI 框架从零建（卡牌拖拽 + 连招链展示 + 文本排版）；骰子概率操控系统的数学正确性；移动端触摸适配 |
| **Art Style** | 2D 手绘风格——卡牌面有塔罗牌式的艺术感 |
| **Art Pipeline Complexity** | 中等——每张卡牌需要独特的插画，但数量可控（60-80 张） |
| **Audio Needs** | 中等——骰子掷出声效、卡牌连锁音效、氛围背景音乐 |
| **Networking** | 无——纯单机 |
| **Content Volume** | ~60-80 张卡牌、8-12 个遭遇类型、3-5 个结局、叙事分支约 20-30 个舍取节点 |

---

## Risks and Open Questions

### Design Risks
- **"概率即武器"的平衡风险**：操控牌太多 → 游戏变枯燥（永远能过）；操控牌不够 → 骰子变随机天气。缓解：MVP 期只放 2-3 张操控牌，数据驱动迭代
- **"得必有失"的心理阻力**：玩家可能不舍得弃牌而产生负面体验。缓解：舍弃动画要做得有仪式感（"一张旧牌化为光点散去"），让失去也成为正向体验

### Technical Risks
- **UI 框架从零建**：卡牌拖拽、连招链可视化、文本排版全部手写。缓解：优先提取 SDL2 轻量 UI 子框架，复用现有 button 模式
- **移动端触摸适配**：SDL2 对移动端不是一等公民。缓解：早期全 PC 开发原型，Tier 3 再切移动端

### Market Risks
- **卡牌 + 骰子已在 Dicey Dungeons / Slay the Spire 中被验证，但移动端叙事卡牌 RPG 的市场大小待确认**
- **审美风险**：塔罗牌/手绘风格的视觉需要在有限预算下执行到位

### Scope Risks
- **独狼 + 6-7 个月 = 内容瓶颈**：60+ 张卡牌 + 全部叙事文本可能超出单人产能。缓解：用数据文件驱动（JSON）替代硬编码，加速内容制作

### Open Questions
- 骰子概率操控的数学平衡——需要一套数值模型在 MVP 前用电子表格/脚本验证
- 移动端 UI 触摸体验——需要 T2 阶段在真机上测试卡牌拖拽手感
- 叙事系统用什么格式——JSON 脚本 vs 自研 DSL？建议 JSON 起步，视情况演进

---

## MVP Definition

**Core hypothesis**: 玩家觉得"先押牌—连招链—操控概率—掷骰引爆"这个核心循环有趣，并且"得必有失"的舍取节点产生情感共鸣。

**Required for MVP**:
1. 一场完整短局（3-5 个连锁遭遇 → 结局）
2. 20-30 张卡牌，每张有骰子检定需求
3. 2-3 张概率操控卡牌
4. 基础骰子系统（1d6 + 视觉动画）
5. 2-3 个"得必有失"舍取节点
6. 手牌区 + 连招槽 + 骰子区的 UI
7. 标题界面 → 一局游戏 → 结算画面

**Explicitly NOT in MVP**:
- 多面骰（d8/d10/d12）—— 仅 d6
- 大量叙事分支
- 移动端适配
- 音频
- 存档系统
- 动画/粒子效果

### Scope Tiers

| Tier | Timeframe | Content | Features |
| ---- | ---- | ---- | ---- |
| **MVP** | 月 1-3 | ~25 张牌、3 舍取节点、3-5 遭遇 | 核心循环：连招→掷骰→结算 |
| **Tier 2 — 内容层** | 月 4-5 | 60-80 张牌、多骰型、8-12 遭遇、叙事分支 | 骰子多样性 + 内容深度 |
| **Tier 3 — 打磨** | 月 6-7 | 最终内容量 | 移动端 + 音频 + 存档 + 多分辨率 |

---

## Visual Identity Anchor

> *此章节在 `/art-bible` 执行期间填写——方向在创意阶段确认。*

---

## Next Steps

- [ ] Run `/setup-engine` — 配置 SDL2 + C++ 为正式引擎栈（更新 CLAUDE.md 和技术偏好）
- [ ] Use `/design-review design/gdd/game-concept.md` 验证概念文档完整性
- [ ] Run `/prototype 连招+掷骰核心循环` — 在写 GDD 之前验证玩法可行性
- [ ] If prototype PROCEEDS: Run `/map-systems` — 分解概念为独立系统
- [ ] Run `/design-system [system-name]` — 为每个系统撰写 GDD
- [ ] Run `/create-architecture` — 撰写主架构蓝图
- [ ] Run `/vertical-slice` — 验证完整游戏循环
