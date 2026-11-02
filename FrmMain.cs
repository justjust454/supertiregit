using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using netDxf.Entities;
using netDxf;
using IO;

namespace UI
{
    // 定义交互模式
 
    public partial class FrmMain : Form
    {
        

        private OCCTProxy myOCCTProxy;
        public FrmMain()
        {
            InitializeComponent();
            myOCCTProxy = new OCCTProxy();
            InitPanel2DLayout();
            InitializeTopologyEditWindow();
        }
        private int componentCounter = 1;        // 记录 Component 的数量
        private TreeNode currentComponentNode = null; 
        // 手工直线的当前目标图层，与面创建使用的 Current Component 相互独立。
        private TreeNode currentLineLayerNode = null;

        // 几何处理统一容差（当前模型单位为 mm）：线端点吸附、线打断和制面共用。
        private double geometryTolerance = 0.01;

        // 模拟 HyperMesh 的高对比度经典色板
        private Color[] hmColorPalette = new Color[] 
        {
            Color.FromArgb(255, 0, 0),     // 经典红
            Color.FromArgb(0, 255, 0),     // 荧光绿
            Color.FromArgb(0, 85, 255),    // 深海蓝
            Color.FromArgb(255, 255, 0),   // 亮黄色
            Color.FromArgb(0, 255, 255),   // 青青色
            Color.FromArgb(255, 0, 255),   // 洋红色
            Color.FromArgb(255, 128, 0),   // 活力橙
            Color.FromArgb(163, 73, 164)   // 幽暗紫
        };


        //DXF文件导入
        private void tsbImport_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = "DXF 图纸 (*.dxf)|*.dxf";
            openFileDialog.Title = "请选择轮胎截面图纸";

            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                string filePath = openFileDialog.FileName;
       

                // 执行导入逻辑
                LoadDxfToOCC(filePath);
            }
        }
        //DXF文件解析
        private void LoadDxfToOCC(string filePath)
        {
            string errorMsg;
            var dxf =IO. DataInteraction.LoadDocument(filePath, out errorMsg);

            if (dxf == null)
            {
                LogMessage(errorMsg); // 打印 IO 层传回来的错误原因
                return;
            }
            globalShapeId = 1;
            currentLineLayerNode = null;
            layerShapesMap.Clear();
            this.treeView1.Nodes.Clear();
            if (myOCCTProxy !=null)
            {
                myOCCTProxy.ClearAll();
            }

            // 绘制直线
            TreeNode rootNode = new TreeNode($"图纸: {System.IO.Path.GetFileName(filePath)}");
            this.treeView1.Nodes.Add(rootNode);
            // 内部辅助方法：根据 DXF 图层名，获取或动态创建子节点
            Dictionary<string, TreeNode> layerNodeCache = new Dictionary<string, TreeNode>();
            TreeNode GetOrCreateLayerNode(string layerName)
            {
                if (!layerNodeCache.ContainsKey(layerName))
                {
                    // 在界面上新建一个图层节点
                    TreeNode newNode = new TreeNode($"图层: {layerName}");
                    layerNodeCache[layerName] = newNode;
                    rootNode.Nodes.Add(newNode);

                    // 只要建了新图层，就立刻在花名册里给它开辟一个专属的“人员名单”(List)
                    layerShapesMap[newNode] = new List<int>();
                }
                return layerNodeCache[layerName];
            }

            int lineCount = 0;
            int arcCount = 0;
            int lwPolylineCount = 0;
            int splineCount = 0;
            // 解析直线并分配 ID
            foreach (var line in dxf.Lines)
            {
                TreeNode layerNode = GetOrCreateLayerNode(line.Layer.Name);
                int currentId = globalShapeId++; // 发放身份证

                if (myOCCTProxy != null)
                {
                    // 将带有 ID 的数据发给 C++ 底层去画图并登记
                    myOCCTProxy.CreateLine(currentId,
                                           line.StartPoint.X, line.EndPoint.X,
                                           line.StartPoint.Y, line.EndPoint.Y,
                                           line.StartPoint.Z, line.EndPoint.Z);
                }

                // 在 C# 端把这个 ID 登记到对应图层的花名册中
                layerShapesMap[layerNode].Add(currentId);
                lineCount++;
            }
            // 解析圆弧并分配 ID
            foreach (var arc in dxf.Arcs)
            {
                TreeNode layerNode = GetOrCreateLayerNode(arc.Layer.Name);
                int currentId = globalShapeId++; // 发放身份证

                if (myOCCTProxy != null)
                {
                    // 将带有 ID 的数据发给 C++ 底层去画图并登记
                    myOCCTProxy.CreateArcByCenter(currentId,
                                                  arc.Center.X, arc.Center.Y, arc.Center.Z,
                                                  arc.Radius, arc.StartAngle, arc.EndAngle);
                }

                // 在 C# 端把这个 ID 登记到对应图层的花名册中
                layerShapesMap[layerNode].Add(currentId);
                arcCount++;
            }
            // 解析轻量多段线 
            foreach (var lwPolyline in dxf.LwPolylines)
            {
                TreeNode layerNode = GetOrCreateLayerNode(lwPolyline.Layer.Name);
                var vertexes = lwPolyline.PolygonalVertexes(50, 0.0, 0.001);

                if (vertexes.Count < 2) continue; // 只有1个点构不成线，跳过

                // 【核心变化】：一整根长线，现在只领 1 张身份证！
                int currentId = globalShapeId++;

                // 把所有离散点塞进一个扁平的一维数组 [x1, y1, z1, x2, y2, z2...]
                List<double> coords = new List<double>();
                foreach (var v in vertexes)
                {
                    coords.Add(v.X);
                    coords.Add(v.Y);
                    coords.Add(0.0); // LwPolyline 是 2D 的，Z 默认为 0
                }

                if (myOCCTProxy != null)
                {
                    // 一次性把所有的点发给 C++ 打包成一根完整的 Wire
                    myOCCTProxy.CreatePolyline(currentId, coords.ToArray(), lwPolyline.IsClosed);
                }

                // 花名册里也只登记这 1 个 ID
                layerShapesMap[layerNode].Add(currentId);
                lwPolylineCount++;
            }
            //解析样条曲线 (Spline)
            foreach (var spline in dxf.Splines)
            {
                TreeNode layerNode = GetOrCreateLayerNode(spline.Layer.Name);
                var vertexes = spline.PolygonalVertexes(100); // 样条曲线精度给高点

                if (vertexes.Count < 2) continue;

                // 【核心变化】：一整根样条曲线，只领 1 张身份证！
                int currentId = globalShapeId++;

                List<double> coords = new List<double>();
                foreach (var v in vertexes)
                {
                    coords.Add(v.X);
                    coords.Add(v.Y);
                    coords.Add(v.Z); // Spline 可能有 Z 坐标
                }

                if (myOCCTProxy != null)
                {
                    myOCCTProxy.CreatePolyline(currentId, coords.ToArray(), spline.IsClosed);
                }

                layerShapesMap[layerNode].Add(currentId);
                splineCount++;
            }





            // 刷新界面与视图
            this.treeView1.ExpandAll(); // 展开树状图，让用户看到所有图层

            if (myOCCTProxy != null)
            {
                myOCCTProxy.ZoomAllView();       // 视角居中
                myOCCTProxy.UpdateCurrentViewer(); // 统一刷新一次 3D 视口，拒绝卡顿
            }
        }

        //OCC界面加载
        private void Form1_Load(object sender, EventArgs e)
        {

            bool isInitSuccess = myOCCTProxy.InitViewer(panel2D.Handle);

            if (!isInitSuccess)
            {
                MessageBox.Show("OCC 三维引擎初始化失败！");
            }
        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {

        }
        
        public void InitView()
        {
            this.myOCCTProxy.InitOCCTProxy();
        }



        //STP文件导出
        private void tsbExport_Click(object sender, EventArgs e)
        {
            if (myOCCTProxy == null) return;

            // 1. 弹出 Windows 标准的保存文件对话框
            using (SaveFileDialog saveDlg = new SaveFileDialog())
            {
                saveDlg.Filter = "STEP 文件 (*.stp;*.step)|*.stp;*.step";
                saveDlg.Title = "将当前可见图形导出为 CAE 几何模型";

                // 默认建议一个文件名，比如叫 Tire_Cleanup.stp
                saveDlg.FileName = "Tire_Section_Mesh.stp";

                if (saveDlg.ShowDialog() == DialogResult.OK)
                {
                    // 2. 更改光标为沙漏状，给用户“正在计算”的良好体验
                    this.Cursor = Cursors.WaitCursor;
                    LogMessage("正在提取视口几何体并生成 B-Rep 拓扑数据...");

                    // 3. 呼叫 C++ 底层发功
                    List<string> componentNames = new List<string>();
                    List<int[]> componentShapeGroups = new List<int[]>();

                    foreach (var layerPair in layerShapesMap)
                    {
                        TreeNode groupNode = layerPair.Key;
                        List<int> shapeIds = layerPair.Value;

                        // 跳过没有任何几何对象的空图层
                        if (groupNode == null ||
                            shapeIds == null ||
                            shapeIds.Count == 0)
                        {
                            continue;
                        }

                        // 清理树节点上用于界面显示的状态文字
                        string groupName = groupNode.Text
                            .Replace(" (Current)", "")
                            .Replace(" [已隐藏]", "")
                            .Trim();

                        // 防止节点名称为空
                        if (string.IsNullOrWhiteSpace(groupName))
                        {
                            groupName = $"GeometryGroup_{componentNames.Count + 1}";
                        }

                        // Component、普通 DXF 图层和帘线图层全部参与导出
                        componentNames.Add(groupName);
                        componentShapeGroups.Add(shapeIds.ToArray());
                    }

                    // C# 的 int[][] 对应 C++/CLI 的 array<array<int>^>^
                    bool isSuccess =
                        componentShapeGroups.Count > 0 &&
                        myOCCTProxy.ExportVisibleToSTEP(
                            saveDlg.FileName,
                            componentNames.ToArray(),
                            componentShapeGroups.ToArray());

                    // 4. 恢复默认光标
                    this.Cursor = Cursors.Default;

                    // 5. 结果反馈
                    if (isSuccess)
                    {
                       
                        LogMessage($"成功成功导出可见图形至 {System.IO.Path.GetFileName(saveDlg.FileName)}");
                    }
                    else
                    {
                        
                        LogMessage("导出失败：未发现可见的业务拓扑对象。");
                    }
                }
            }
        }

        //OCC显示区域初始化
        private void InitPanel2DLayout()
        {
            // 1. 让 panel2D 自动填满它的父容器（例如 SplitContainer 的 Panel2）
            // 这样无论你拉动拆分条还是最大化窗口，panel2D 的 Size 都会自动随之精确改变
            this.panel2D.Dock = DockStyle.Fill;

            // 2. 显式绑定 panel2D 的 Resize 事件
            this.panel2D.Resize += new EventHandler(Panel2D_Resize);
            // 【新增】：绑定鼠标交互事件
            this.panel2D.MouseDown += new MouseEventHandler(Panel2D_MouseDown);
            this.panel2D.MouseMove += new MouseEventHandler(Panel2D_MouseMove);
            this.panel2D.MouseUp += new MouseEventHandler(Panel2D_MouseUp);
            this.panel2D.MouseWheel += new MouseEventHandler(Panel2D_MouseWheel);
            this.panel2D.MouseEnter += new EventHandler(Panel2D_MouseEnter);
        }
        
        /// <summary>
        //OCC显示区域大小调整
        /// <summary>
        private void Panel2D_Resize(object sender, EventArgs e)
        {
            // 确保底层代理类已经初始化
            if (myOCCTProxy != null)
            {
                // 3. 通知底层 C++：视口窗口大小变了，需要重新适配相机比例
                // 对应你 C++ 代码里的 myView()->MustBeResized();
                myOCCTProxy.UpdateView();

                // 4. 紧接着让底层视图重新绘制画面，防止拉伸过程中产生残影或黑边
                // 对应你 C++ 代码里的 myView()->Redraw();
                myOCCTProxy.RedrawView();
            }
        }

        private void richTextBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void richTextBox1_TextChanged_1(object sender, EventArgs e)
        {

        }
        //日志输出
        private void LogMessage(string msg)
        {
            // richTextBoxLog 是你需要在界面上拖出的一个 RichTextBox 控件的名字
            if (richTextBoxLog != null)
            {
                richTextBoxLog.AppendText($"[{DateTime.Now:HH:mm:ss}] {msg}\n");
                richTextBoxLog.ScrollToCaret(); // 自动滚动到最新一行
            }
        }
        
        //新建任务
        private void tsbNew_Click(object sender, EventArgs e)
        {
         
                // 2. 清空 OCC 引擎底层的三维几何体
                if (myOCCTProxy != null)
                {
                    myOCCTProxy.ClearAll();      // 调用我们刚写的 C++ 接口
                    myOCCTProxy.Reset();         // 重置相机视角到默认状态
                }

                // 3. 清空左侧的模型树 
                 if (this.treeView1 != null)
                 {
                     this.treeView1.Nodes.Clear();
                 }

                // 4. 清理底部的日志窗口并输出提示
                if (richTextBoxLog != null)
                {
                    richTextBoxLog.Clear(); // 清空之前的冗长日志
                    LogMessage("已新建空白文件，工作区已重置。");
                }
        }

        private void panel1_Paint_1(object sender, PaintEventArgs e)
        {

        }
        
        
        //删除线条按钮
        private void tsbDelete_Click(object sender, EventArgs e)
        {
            if (currentMode != InteractionMode.DeleteLines)
            {
                SwitchInteractionMode(InteractionMode.DeleteLines, tsbDelete);
                LogMessage("进入删除模式：请直接点击需要删除的线段 。");
            }
            else
            {
                SwitchInteractionMode(InteractionMode.Normal, null);
                LogMessage("已退出删除模式。");
            }
        }

        private void treeView1_AfterSelect(object sender, TreeViewEventArgs e)
        {

        }


        //合并线条按钮
        private void tsbAutoCombine_Click(object sender, EventArgs e)
        {
            if (currentMode != InteractionMode.AutoCombineLines)
            {
                SwitchInteractionMode(InteractionMode.AutoCombineLines, tsbAutoCombine);
                LogMessage("进入线条合并模式：请在视图中点选第 1 条直线。");
            }
            else // 再次点击自己，相当于退出模式
            {
                SwitchInteractionMode(InteractionMode.Normal, null);
                LogMessage("已退出合并模式。");
            }
        }

        //打断线条按钮
        private void tsbSplit_Click(object sender, EventArgs e)
        {
            if (currentMode != InteractionMode.SplitLines)
            {
                SwitchInteractionMode(InteractionMode.SplitLines, tsbSplit);
                LogMessage("进入打断模式：请选择需要被打断的目标线段。");
            }
            else
            {
                SwitchInteractionMode(InteractionMode.Normal, null);
                LogMessage("已退出打断模式。");
            }
        }

        private void tsbSurfaceCreate_Click(object sender, EventArgs e)
        {
            if (currentMode != InteractionMode.MakeFace)
            {
                SwitchInteractionMode(InteractionMode.MakeFace,tsbSurfaceCreate);
                LogMessage("进入面创建模式");
            }
            else
            {
                SwitchInteractionMode(InteractionMode.Normal, null);
                LogMessage("已退出面创建模式。");
            }
        }

        private void btnFaceCancel_Click(object sender, EventArgs e)
        {
            SwitchInteractionMode(InteractionMode.Normal, null);
            LogMessage("已取消面创建操作。");
        }

        private void btnFaceCreate_Click(object sender, EventArgs e)
        {
            // 1. 【核心防御】：如果没有激活的 Component，严禁创建面！
            if (currentComponentNode == null)
            {
                MessageBox.Show("未找到当前组件！\n请先在左侧树状图中右键“图层基体”->“新建组件”，或右键已有组件选择“设为当前 (Make Current)”。",
                                "无活动组件", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            // 2. 线段数量防御
            if (selectedLinesForFace.Count == 0)
            {
                MessageBox.Show("请至少选择一条能够形成闭合区域的线！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            int newFaceId = globalShapeId++;

            // 3. 直接提取【当前组件】的专属颜色
            Color compColor = currentComponentNode.ForeColor;

            // 4. 呼叫 C++ 底层发功，带上 RGB 颜色值
            bool success = myOCCTProxy.CreateFaceFromLines(
                selectedLinesForFace.ToArray(), newFaceId,
                compColor.R, compColor.G, compColor.B,
                geometryTolerance);

            if (success)
            {
                // 5. 将新生成的面放入当前 Active 的 Component 树节点中
                TreeNode faceNode = new TreeNode($"面 {faceCounter++}")
                {
                    Tag = newFaceId,
                    ForeColor = compColor
                };
                currentComponentNode.Nodes.Add(faceNode);

                // 将面的 ID 登记在当前 Component 的花名册里
                layerShapesMap[currentComponentNode].Add(newFaceId);

                currentComponentNode.Expand(); // 展开节点展示成果

                LogMessage($"面创建成功！已保存至当前组件: {currentComponentNode.Text.Replace(" (Current)", "")}");

                // 自动重置排队名单，等待下一个面的创建
                selectedLinesForFace.Clear();
            }
            else
            {
                LogMessage("面创建失败：所选线条没有形成可识别的闭合区域，请检查是否共面或存在交叉分叉。");
                globalShapeId--; // 失败了把 ID 还回去
            }
        }

        private void tsbCombine_Click(object sender, EventArgs e)
        {
            if (currentMode != InteractionMode.CombineLines)
            {
                SwitchInteractionMode(InteractionMode.CombineLines, tsbCombine);
                LogMessage("进入线条合并模式：请在视图中点选第 1 条直线。");
            }
            else // 再次点击自己，相当于退出模式
            {
                SwitchInteractionMode(InteractionMode.Normal, null);
                LogMessage("已退出合并模式。");
            }
        }

        private void tsbCordCreate_Click(object sender, EventArgs e)
        {
            if (currentMode != InteractionMode.CordCreate)
            {
                SwitchInteractionMode(InteractionMode.CordCreate, tsbCordCreate);
                LogMessage("进入帘线创建模式：请在视图中点选第 1 条直线。");
            }
            else // 再次点击自己，相当于退出模式
            {
                SwitchInteractionMode(InteractionMode.Normal, null);
                LogMessage("已退出帘线创建模式。");
            }
        }

        private void tsbLineCreate_Click(object sender, EventArgs e)
        {
            if (currentMode == InteractionMode.LineCreate)
            {
                SwitchInteractionMode(InteractionMode.Normal, null);
                LogMessage("已退出直线绘制模式，已完成的直线予以保留。");
                return;
            }

            EnterLineCreateMode();
        }

        private void EnterLineCreateMode()
        {
            if (currentMode == InteractionMode.TopologyEdit)
                FinishTopologyEditSession();

            if (!IsLineDrawingLayerNode(currentLineLayerNode))
            {
                MessageBox.Show(
                    "尚未设置当前绘图图层。\n" +
                    "请在左侧模型树中右键任一图层，选择“设置为当前 (Make Current)”。",
                    "直线绘制",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Warning);
                return;
            }

            if (myOCCTProxy == null ||
                !myOCCTProxy.BeginLineCreate(geometryTolerance))
            {
                MessageBox.Show(
                    "无法进入直线绘制模式，请确认二维视图已经初始化。",
                    "直线绘制",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Warning);
                return;
            }

            SwitchInteractionMode(
                InteractionMode.LineCreate,
                tsbLineCreate);

            LogMessage(
                $"进入直线绘制模式，直线将保存到 {GetLineLayerName(currentLineLayerNode)}。" +
                "请指定第一点，移动鼠标预览后再指定第二点；靠近现有线段时会自动吸附。");
        }

        private void HandleLineCreateClick(int x, int y)
        {
            int[] result = myOCCTProxy.AddLineCreatePoint(
                x,
                y,
                globalShapeId);

            if (result == null || result.Length < 2)
            {
                LogMessage("直线绘制失败：底层没有返回有效结果。");
                return;
            }

            switch (result[0])
            {
                case 1:
                    LogMessage("已确定直线第一点，请指定第二点。");
                    break;

                case 2:
                    // 每次完成直线时读取当前图层，因此绘制期间执行 Make Current
                    // 也会从下一条直线开始立即生效。
                    layerShapesMap[currentLineLayerNode].Add(result[1]);
                    globalShapeId = Math.Max(globalShapeId, result[1] + 1);
                    LogMessage(
                        $"直线绘制成功（ID: {result[1]}），已保存到 " +
                        $"{GetLineLayerName(currentLineLayerNode)}。" +
                        "按 Esc 或再次点击按钮退出。");
                    break;

                case 0:
                    LogMessage("当前光标无法投影到图纸平面，请调整视角后重试。");
                    break;

                case -2:
                    LogMessage("两个点距离过近，请重新指定第二点。");
                    break;

                case -4:
                    LogMessage("新直线 ID 已被占用，请重试。");
                    break;

                default:
                    LogMessage("OCC 直线创建失败，请重新指定两个点。");
                    break;
            }
        }

        private FrmTopologyEdit topologyEditForm;
        private TreeNode topologyEditingComponentNode;
        private List<int> topologyOriginalShapeIds =
            new List<int>();

        private void InitializeTopologyEditWindow()
        {
            topologyEditForm =
                new FrmTopologyEdit();

            topologyEditForm.Tolerance =
                geometryTolerance;

            topologyEditForm.FinishRequested +=
                topologyEditForm_FinishRequested;

            topologyEditForm.CancelRequested +=
                topologyEditForm_CancelRequested;
        }

        private void tsbTopologyEdit_Click_1(object sender, EventArgs e)
        {
            if (currentMode ==InteractionMode.TopologyEdit)
            {
                FinishTopologyEditSession();
            }
            else
            {
                EnterTopologyEditMode();
            }
        }

        private void topologyEditForm_FinishRequested(object sender,EventArgs e)
        {
            FinishTopologyEditSession();
        }

        private void topologyEditForm_CancelRequested(object sender,EventArgs e)
        {
            CancelTopologyEditSession();
        }

        private void EnterTopologyEditMode()
        {
            if (currentComponentNode == null ||
                !currentComponentNode.Text.StartsWith(
                    "Component",
                    StringComparison.Ordinal))
            {
                MessageBox.Show(
                    "请先在模型树中选择需要进行拓扑编辑的 Component。",
                    "拓扑编辑",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Warning);

                return;
            }

            List<int> componentShapeIds;

            if (!layerShapesMap.TryGetValue(
                currentComponentNode,
                out componentShapeIds) ||
                componentShapeIds == null ||
                componentShapeIds.Count == 0)
            {
                MessageBox.Show(
                    "当前 Component 中没有可以进行拓扑编辑的面。",
                    "拓扑编辑",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Warning);

                return;
            }

            double tolerance =
                 topologyEditForm.Tolerance;

            int[] shapeIds =
                componentShapeIds.ToArray();

            bool success =
                myOCCTProxy.BeginTopologyEdit(
                    shapeIds,
                    tolerance);

            if (!success)
            {
                MessageBox.Show(
                    "没有找到可以编辑的面拓扑边。",
                    "拓扑编辑",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Warning);

                return;
            }

            topologyEditingComponentNode =
                currentComponentNode;

            topologyOriginalShapeIds =
                new List<int>(shapeIds);

            SwitchInteractionMode(
                InteractionMode.TopologyEdit,
                 tsbTopologyEdit);

            string componentName =
                currentComponentNode.Text
                .Replace(" (Current)", "")
                .Replace(" [已隐藏]", "");

            topologyEditForm.ComponentName =componentName;

            topologyEditForm.CurrentEdgeText = "尚未选择";

            topologyEditForm.StatusText ="请选择一条红色自由边。\r\n" +"程序将自动寻找匹配边并完成缝合。";

            topologyEditForm.ShowFor(this);

            LogMessage(
                "进入拓扑编辑模式：请选择需要缝合的红色自由边。");
        }


        // 在 Panel2D_MouseUp 中点击拓扑覆盖边后调用
        private void HandleTopologyEdgeClick()
        {
            if (myOCCTProxy == null)
                return;

            double tolerance =
                topologyEditForm.Tolerance;

            int[] result =
                myOCCTProxy.SewSelectedTopologyEdge(
                    tolerance);

            if (result == null || result.Length < 3)
            {
                topologyEditForm.StatusText =
                    "拓扑操作失败：底层没有返回有效结果。";
                return;
            }

            int status = result[0];
            int keptShapeId = result[1];
            int removedShapeId = result[2];

            switch (status)
            {
                // 红色自由边缝合成功，生成绿色共享边
                case 1:
                    {
                        if (topologyEditingComponentNode != null &&
                            layerShapesMap.ContainsKey(
                                topologyEditingComponentNode))
                        {
                            List<int> currentIds =
                                layerShapesMap[
                                    topologyEditingComponentNode];

                            if (removedShapeId >= 0)
                                currentIds.Remove(removedShapeId);

                            if (!currentIds.Contains(keptShapeId))
                                currentIds.Add(keptShapeId);
                        }

                        topologyEditForm.CurrentEdgeText ="绿色共享边";

                        topologyEditForm.StatusText =
                            "边缝合成功。\r\n" +
                            "对应的两条自由边已经变成一条绿色共享边。";

                        LogMessage(
                            $"拓扑操作成功：保留 Shape {keptShapeId}，" +
                            $"合并 Shape {removedShapeId}。");

                        break;
                    }

                // 点击的边已经是共享边
                case 2:
                    {
                        topologyEditForm.CurrentEdgeText ="绿色共享边";

                        topologyEditForm.StatusText =
                            "当前边已经是共享边，不需要重复缝合。";

                        LogMessage("当前拓扑边已经完成缝合。");
                        break;
                    }

                case 0:
                    {
                        topologyEditForm.StatusText =
                            "没有选中拓扑边，请直接点击红色自由边。";
                        break;
                    }

                case -1:
                    {
                        topologyEditForm.StatusText =
                            $"没有找到匹配边。\r\n" +
                            $"请检查间隙是否超过 {tolerance:0.####} mm。";

                        LogMessage("拓扑缝合失败：没有找到匹配自由边。");
                        break;
                    }

                case -2:
                    {
                        topologyEditForm.StatusText =
                            "找到多个相似的候选边，无法自动确定目标。";

                        LogMessage("拓扑缝合失败：匹配边存在歧义。");
                        break;
                    }

                case -3:
                    {
                        topologyEditForm.StatusText =
                            "OCC 缝合失败，或者缝合后没有形成共享边。";

                        LogMessage("拓扑缝合失败：没有形成有效共享边。");
                        break;
                    }

                default:
                    {
                        topologyEditForm.StatusText =
                            "当前对象不是有效拓扑边。";
                        break;
                    }
            }

            myOCCTProxy.UpdateCurrentViewer();
        }


        private void FinishTopologyEditSession()
        {
            if (myOCCTProxy != null)
                myOCCTProxy.FinishTopologyEdit();

            topologyEditForm.HideToolWindow();
            SwitchInteractionMode(
                InteractionMode.Normal,
                null,true);

            topologyEditingComponentNode = null;
            topologyOriginalShapeIds.Clear();

            LogMessage("拓扑编辑完成。");
        }


        private void CancelTopologyEditSession()
        {
            if (myOCCTProxy != null)
                myOCCTProxy.CancelTopologyEdit();

            // 恢复 C# Component 中原来的 Shape ID
            if (topologyEditingComponentNode != null &&
                layerShapesMap.ContainsKey(
                    topologyEditingComponentNode))
            {
                List<int> currentIds =
                    layerShapesMap[topologyEditingComponentNode];

                currentIds.Clear();
                currentIds.AddRange(
                    topologyOriginalShapeIds);
            }

            topologyEditForm.HideToolWindow();
            SwitchInteractionMode(
                InteractionMode.Normal,
                null,true);

            topologyEditingComponentNode = null;
            topologyOriginalShapeIds.Clear();

            LogMessage("已取消拓扑编辑，原始面已经恢复。");
        }
    }
}
