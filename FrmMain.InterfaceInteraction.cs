using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.Windows.Forms;

namespace UI
{
    internal class InterfaceInteraction { }
    public partial class FrmMain
    {
        private System.Drawing.Point lastMousePos;
        private System.Drawing.Point mouseDownPos;

        private enum InteractionMode
        {
            Normal,         // 正常模式
            CombineLines,    //手动线条合并
            AutoCombineLines,   // 线条自动合并模式
            SplitLines,      //线条打断模式
            DeleteLines,      //线条删除模式
            CordCreate,        //
            MakeFace         //面创建
        }
        private InteractionMode currentMode = InteractionMode.Normal;
        private List<int> selectedLinesForCombine = new List<int>(); // 暂存选中的线ID
        private List<int> selectedLinesForSplit = new List<int>();
        private List<int> selectedLinesForDelete = new List<int>();
        private List<int> selectedLinesForFace = new List<int>();
        private List<int> selectedLinesForAutoCombine = new List<int>();
        private List<int> selectedLinesForCord = new List<int>();




        private int faceCounter = 1; // 记录生成了几个面，用于命名

        // 增加两个布尔值，专门负责“相机控制”
        private bool isRotating = false;
        private bool isPanning = false;

        /// <summary>
        /// 统一的业务状态切换中心（互斥开关）
        /// </summary>
        /// <param name="newMode">想要进入的新模式</param>
        /// <param name="activeButton">触发该模式的 ToolStripButton</param>
        private void SwitchInteractionMode(InteractionMode newMode, ToolStripButton activeButton)
        {
            // 1. 底层数据大清洗：无论切换到什么模式，先把上一个模式的残留数据全部清空
            selectedLinesForCombine.Clear();
            selectedLinesForSplit.Clear();
            selectedLinesForFace.Clear(); // 清空选面队列
            selectedLinesForDelete.Clear();
            selectedLinesForAutoCombine.Clear();
            selectedLinesForCord.Clear();
            if (myOCCTProxy != null)
            {
                myOCCTProxy.ClearSelected();
                myOCCTProxy.UpdateCurrentViewer(); // 强制刷新画面
            }

            // 2. 赋值新的底层模式
            currentMode = newMode;

            // 3. UI 按钮状态互斥管理 (只有一个能凹下去，或者全弹起来)
            tsbAutoCombine.Checked = (activeButton == tsbAutoCombine);
            tsbSplit.Checked = (activeButton == tsbSplit);
            tsbDelete.Checked = (activeButton == tsbDelete);
            tsbSurfaceCreate.Checked = (activeButton == tsbSurfaceCreate);
            tsbCombine.Checked = (activeButton == tsbCombine);
            tsbCordCreate.Checked = (activeButton == tsbCordCreate);

            // 4. 鼠标指针统一管理
            if (newMode == InteractionMode.Normal)
            {
                this.panel2D.Cursor = Cursors.Default;
            }
            else
            {
                this.panel2D.Cursor = Cursors.Cross; // 合并/打断用十字准星
            }
            if (pnlFaceAction != null)
            {
                pnlFaceAction.Visible = (newMode == InteractionMode.MakeFace);

            }
        }





        /// <summary>
        /// 滚轮监听
        /// </summary>
        private void Panel2D_MouseEnter(object sender, EventArgs e)
        {
            this.panel2D.Focus();
        }

        /// <summary>
        /// 鼠标按下事件：记录起点，激活相机拖拽状态
        /// </summary>
        private void Panel2D_MouseDown(object sender, MouseEventArgs e)
        {
            lastMousePos = e.Location;
            mouseDownPos = e.Location;

            if (e.Button == MouseButtons.Left)
            {
                isRotating = true; // 左键按下，旋转
                if (myOCCTProxy != null)
                {
                    myOCCTProxy.StartRotation(e.X, e.Y);
                }
            }
            else if (e.Button == MouseButtons.Middle)
            {
                isPanning = true; // 中键按下，平移
            }
        }

        /// <summary>
        /// 鼠标移动事件：优先处理拖拽，否则执行拾取(高亮)
        /// </summary>
        private void Panel2D_MouseMove(object sender, MouseEventArgs e)
        {
            if (myOCCTProxy == null) return;

            // 如果鼠标处于按住拖拽的状态，执行相机视角变换
            if (isRotating)
            {
                myOCCTProxy.Rotation(e.X, e.Y);
            }
            else if (isPanning)
            {
                int deltaX = e.X - lastMousePos.X;
                int deltaY = e.Y - lastMousePos.Y;
                myOCCTProxy.Pan(deltaX, -deltaY);
            }
            // 只要鼠标没按住拖拽，无论什么模式，都执行探测高亮！
            else
            {
                myOCCTProxy.MoveTo(e.X, e.Y);
            }

            lastMousePos = e.Location;
        }

        /// <summary>
        /// 鼠标抬起事件
        /// </summary>
        private void Panel2D_MouseUp(object sender, MouseEventArgs e)
        {
            // 鼠标抬起了，解除相机的拖拽状态
            if (e.Button == MouseButtons.Left) isRotating = false;
            if (e.Button == MouseButtons.Middle) isPanning = false;

            if (myOCCTProxy != null && e.Button == MouseButtons.Left)
            {
                // 核心判断：是“点击”，还是“拖拽旋转”
                int dx = Math.Abs(e.X - mouseDownPos.X);
                int dy = Math.Abs(e.Y - mouseDownPos.Y);

                if (dx < 3 && dy < 3)
                {
                    // === 点击：执行 OCC 选中高亮 ===
                    if ((Control.ModifierKeys & Keys.Shift) == Keys.Shift)
                        myOCCTProxy.ShiftSelect();
                    else
                        myOCCTProxy.Select();

                    // === 业务状态 ===
                    //===线条自动合并===
                    if (currentMode == InteractionMode.AutoCombineLines)
                    {
                        myOCCTProxy.Select(); // 选中鼠标下的第一根线
                        myOCCTProxy.UpdateCurrentViewer();

                        List<int> selIds = myOCCTProxy.GetSelectedIds();

                        if (selIds.Count > 0)
                        {
                            int startId = selIds[0];
                            int newId = globalShapeId++;
                            
                            int[] consumedIds = myOCCTProxy.AutoCombineLines(startId, newId);

                            if (consumedIds != null && consumedIds.Length > 0)
                            {
                                LogMessage($"已将 {consumedIds.Length} 条碎线自动连结为 1 根平滑曲线。");
                                // 更新左侧的树状图
                                UpdateTreeAfterAutoCombine(consumedIds, newId);
                            }
                            else
                            {
                                LogMessage("该线段周围没有发现相连的其他线条。");
                            }
                        }
                    }
                    //===线条手动合并===
                    if(currentMode == InteractionMode.CombineLines)
                    {
                        myOCCTProxy.Select();
                        myOCCTProxy.UpdateCurrentViewer();

                        List<int> selIds = myOCCTProxy.GetSelectedIds();

                        foreach(int clickedId in selIds)
                        {
                            if(!selectedLinesForCombine.Contains(clickedId))
                            {
                                selectedLinesForCombine.Add(clickedId);
                                LogMessage($"已选取ID:{clickedId}线条，其继续选择第二条线");
                            }
                        }
                        if(selectedLinesForCombine.Count == 2)
                        {
                            int id1 = selectedLinesForCombine[0];
                            int id2 = selectedLinesForCombine[1];
                            int newId = globalShapeId++;
                            bool success = myOCCTProxy.CombineLines(id1, id2, newId);
                            if (success)
                            {
                                LogMessage("合并成功！已生成连续多段线。");
                                UpdateTreeAfterCombine(id1, id2, newId);
                            }
                            else
                            {
                                LogMessage("合并失败：所选图形不支持或距离过远。");
                            }

                            selectedLinesForCombine.Clear();

                        }
                        
                    }
                



                    // === 线条打断 ===
                    else if (currentMode == InteractionMode.SplitLines)
                    {
                        myOCCTProxy.Select();
                        myOCCTProxy.UpdateCurrentViewer();

                        List<int> selIds = myOCCTProxy.GetSelectedIds();

                        if (selIds.Count > 0)
                        {
                            int clickedId = selIds[0]; // 获取点中的线

                            if (selectedLinesForSplit.Count == 0)
                            {
                                // 第一下点击：选中被打断线条
                                selectedLinesForSplit.Add(clickedId);
                                LogMessage($"已拾取目标 (ID:{clickedId})。请继续点击与其相交的线段...");
                            }
                            else if (selectedLinesForSplit.Count == 1)
                            {
                                // 第二下点击：选择与被打断线条相交的线条
                                int targetId = selectedLinesForSplit[0];
                                int toolId = clickedId;

                                if (targetId == toolId)
                                {
                                    LogMessage("请选择另一条相交的线。");
                                    return;
                                }

                                // 触发打断指令
                                int[] newIds = myOCCTProxy.SplitLine(targetId, toolId, globalShapeId);

                                if (newIds != null && newIds.Length > 0)
                                {
                                    // 成功了，扣除消耗掉的 ID 份额
                                    globalShapeId += newIds.Length;
                                    LogMessage($"原线条被切分为 {newIds.Length} 段。");

                                    // 更新树状图
                                    UpdateTreeAfterSplit(targetId, newIds);
                                }
                                else
                                {
                                    LogMessage("打断失败：两根线条在空间中并没有真实的物理交点！");
                                }

                                // 无论成功还是失败，清空队列，准备下一轮打断
                                selectedLinesForSplit.Clear();
                                LogMessage("---------------------------------------");
                                LogMessage("请点击下一条需要被打断的线或退出。");
                            }
                        }
                    }
                    //===删除线条模式===
                    else if (currentMode == InteractionMode.DeleteLines)
                    {
                        myOCCTProxy.Select();
                        myOCCTProxy.UpdateCurrentViewer();
                        List<int> selIds = myOCCTProxy.GetSelectedIds();

                        if (selIds.Count > 0)
                        {
                            int clickedId = selIds[0];

                            // 触发OCC删除指令
                            bool success = myOCCTProxy.DeleteLine(clickedId);

                            if (success)
                            {
                                LogMessage($"成功删除线条 (ID: {clickedId})。");

                                // 更新前端的图层数据字典
                                UpdateTreeAfterDelete(clickedId);
                            }
                        }
                    }
                    // === 面创建模式 ===
                    else if (currentMode == InteractionMode.MakeFace)
                    {
                        if (e.Button == MouseButtons.Left)
                        {
                            // 1. 回归最稳的单选模式，探测鼠标精确点中了谁
                            myOCCTProxy.Select();
                            List<int> newlyClicked = myOCCTProxy.GetSelectedIds();

                            // 2. 如果真的点中了一根线
                            if (newlyClicked.Count > 0)
                            {
                                int clickedId = newlyClicked[0];

                                // 3. C# 自己维护累加逻辑，绝对不会丢！
                                if (!selectedLinesForFace.Contains(clickedId))
                                {
                                    selectedLinesForFace.Add(clickedId);
                                }
                                else
                                {
                                    LogMessage("此线段已被选过，请点击下一条。");
                                }
                            }
                            else
                            {
                                // 如果点到了空白处，什么都不做，防止丢失已有选择
                            }

                            myOCCTProxy.ClearSelected(); 

                            foreach (int id in selectedLinesForFace)
                            {
                                myOCCTProxy.AddToSelection(id); 
                            }

                            myOCCTProxy.UpdateCurrentViewer(); 

                            LogMessage($"已选中 {selectedLinesForFace.Count} 条线段。左键继续累加，点击 Confirm 生成。");
                        }
                        
                    }
                    //===帘线创建模式===
                    else if (currentMode == InteractionMode.CordCreate)
                    {
                        if (e.Button == MouseButtons.Left)
                        {
                            // 1. 探测鼠标点中了谁
                            myOCCTProxy.Select();
                            myOCCTProxy.UpdateCurrentViewer();
                            List<int> selIds = myOCCTProxy.GetSelectedIds();

                            // 2. 如果真的点中了一根线
                            if (selIds.Count > 0)
                            {
                                int clickedId = selIds[0];

                                // 3. 防止重复点同一根线
                                if (!selectedLinesForCord.Contains(clickedId))
                                {
                                    selectedLinesForCord.Add(clickedId);

                                    // 如果这是第一根线
                                    if (selectedLinesForCord.Count == 1)
                                    {
                                        LogMessage($"已选取边界曲线 A (ID: {clickedId})。请继续点击对应的边界曲线 B...");
                                    }
                                    // 如果两根线都选齐了，直接在这里召唤神龙！
                                    else if (selectedLinesForCord.Count == 2)
                                    {
                                        LogMessage($"已选取边界曲线 B (ID: {clickedId})。正在计算中位帘线...");

                                        // ==========================================
                                        // 【直接集成的生成逻辑】
                                        // ==========================================
                                        TreeNode activeNode = treeView1.SelectedNode;

                                        // 严苛约束：必须先在树状图选中帘线图层，不然不知道生成的线存哪
                                        if (activeNode == null || !activeNode.Text.Contains("图层：帘线"))
                                        {
                                            MessageBox.Show("层级归属错误：\n请先在左侧树状图空白处右键【新建帘线图层】",
                                                            "操作提示", MessageBoxButtons.OK, MessageBoxIcon.Warning);

                                            // 报错了就把刚选的线清空，让用户重新选
                                            selectedLinesForCord.Clear();
                                            return;
                                        }

                                        // 把暂存的两根线的ID拿出来
                                        int id1 = selectedLinesForCord[0];
                                        int id2 = selectedLinesForCord[1];
                                        int newCordId = globalShapeId++;

                                        // 呼叫 C++ 底层发功，进行离散求交拟合
                                        bool isSuccess = myOCCTProxy.CreateCordFromTwoCurves(id1, id2, newCordId);

                                        if (isSuccess)
                                        {
                                            // 1. 内存花名册里登记造册
                                            layerShapesMap[activeNode].Add(newCordId);

                                            // 2. 左侧树状图里生成新节点（强制绿色）
                                            TreeNode cordNode = new TreeNode($"帘线_{newCordId}");
                                            cordNode.ForeColor = Color.Green;

                                            activeNode.Nodes.Add(cordNode);
                                            activeNode.Expand(); // 自动展开树让用户看到

                                            LogMessage($"生成成功");
                                        }
                                        else
                                        {
                                            LogMessage(" 生成失败");
                                            globalShapeId--; 
                                        }
                                        selectedLinesForCord.Clear();
                                        // ==========================================
                                    }
                                }
                                else
                                {
                                    LogMessage("该线条已被选择，请选择另一条");
                                }
                            }
                            else
                            {

                            }
                        }
                    }



                }
            }
        }
        private void Panel2D_MouseWheel(object sender, MouseEventArgs e)
        {
            if (myOCCTProxy == null) return;

            int cursorX = e.X;
            int cursorY = e.Y;

            // 构造虚拟起点 (x1, y1)，使其距离鼠标点固定为 100 像素
            int x1 = cursorX + 100;
            int y1 = cursorY;

            // e.Delta > 0 代表滚轮向前滚动
            if (e.Delta > 0)
            {
                // 【放大】：距离变为 115 (1.15倍)
                int x2 = cursorX + 115;
                int y2 = cursorY;
                myOCCTProxy.Zoom(x1, y1, x2, y2);
            }
            else
            {
                // 【缩小】：距离变为 85 (0.85倍)
                int x2 = cursorX + 85;
                int y2 = cursorY;
                myOCCTProxy.Zoom(x1, y1, x2, y2);
            }
        }
        /// <summary>
        /// 在基体图层下新建一个 Component，并自动分配颜色
        /// </summary>
        private void CreateNewComponent(TreeNode baseNode)
        {
            Color compColor = hmColorPalette[(componentCounter - 1) % hmColorPalette.Length];
            TreeNode compNode = new TreeNode($"Component {componentCounter}");

            // 赋予专属颜色
            compNode.ForeColor = compColor;

            baseNode.Nodes.Add(compNode);

            // 在底层花名册中开户
            layerShapesMap[compNode] = new List<int>();

            componentCounter++;
            baseNode.Expand();

            // 【极佳体验】：新建组件后，默认将其设为当前 (Make Current)
            SetCurrentComponent(compNode);
            LogMessage($"已创建并激活新组件: {compNode.Text.Replace(" (Current)", "")}");
        }
        /// <summary>
        /// 核心逻辑：设置当前活跃的 Component (Make Current)
        /// </summary>
        private void SetCurrentComponent(TreeNode targetNode)
        {
            // 1. 如果之前有 Current 组件，把它恢复成普通样式
            if (currentComponentNode != null)
            {
                currentComponentNode.Text = currentComponentNode.Text.Replace(" (Current)", "");
                currentComponentNode.NodeFont = new Font(treeView1.Font, FontStyle.Regular);
            }

            // 2. 将目标组件设为 Current
            currentComponentNode = targetNode;

            // 3. UI 视觉强化：加粗并加上 (Current) 后缀，完全致敬 HyperMesh
            currentComponentNode.Text += " (Current)";
            currentComponentNode.NodeFont = new Font(treeView1.Font, FontStyle.Bold);

            LogMessage($"当前活动组件已切换为: {targetNode.Text}");
        }










    }
}
