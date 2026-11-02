using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace UI
{

    public partial class FrmMain
    {
        private int globalShapeId = 1;
        private Dictionary<TreeNode, List<int>> layerShapesMap = new Dictionary<TreeNode, List<int>>();
        //treeView显示图层
        private void UpdateModelTree(netDxf.DxfDocument dxf, string fileName)
        {
            if (this.treeView1 == null) return;

            // 挂起界面绘制，防止大量添加节点时界面闪烁和卡死 (极其重要！)
            this.treeView1.BeginUpdate();

            // 1. 清空旧树
            currentLineLayerNode = null;
            this.treeView1.Nodes.Clear();

            // 2. 创建根节点 (显示文件名)
            string shortName = System.IO.Path.GetFileName(fileName);
            TreeNode rootNode = new TreeNode($"轮胎模型: {shortName}");
            this.treeView1.Nodes.Add(rootNode);

            // 3. 使用字典来临时存储图层节点，避免重复创建同一个图层
            Dictionary<string, TreeNode> layerNodes = new Dictionary<string, TreeNode>();

            // 辅助函数：根据图层名获取对应节点，如果没有则创建
            TreeNode GetOrCreateLayerNode(string layerName)
            {
                if (!layerNodes.ContainsKey(layerName))
                {
                    TreeNode newLayerNode = new TreeNode($"图层: {layerName}");
                    layerNodes[layerName] = newLayerNode;
                    rootNode.Nodes.Add(newLayerNode);
                }
                return layerNodes[layerName];
            }

            // 4. 遍历提取直线并挂载到对应图层

            foreach (var line in dxf.Lines)
            {
                GetOrCreateLayerNode(line.Layer.Name);
            }

            // 5. 遍历提取圆弧并挂载到对应图层

            foreach (var arc in dxf.Arcs)
            {
                GetOrCreateLayerNode(arc.Layer.Name); ;
            }

            // 6. 展开根节点和所有图层节点，方便用户查看
            rootNode.ExpandAll();

            // 恢复界面绘制
            this.treeView1.EndUpdate();
        }
        // ==========================================
        // 右键菜单对象
        // ==========================================
        private System.Windows.Forms.ContextMenuStrip layerContextMenu;

        /// <summary>
        /// 初始化图层的右键菜单 
        /// </summary>
        private void InitLayerContextMenu()
        {
            layerContextMenu = new System.Windows.Forms.ContextMenuStrip();

            var showItem = new System.Windows.Forms.ToolStripMenuItem(" 显示");
            var hideItem = new System.Windows.Forms.ToolStripMenuItem(" 隐藏");

            // 绑定点击事件
            hideItem.Click += (s, e) => ToggleLayerVisibility(false);
            showItem.Click += (s, e) => ToggleLayerVisibility(true);

            layerContextMenu.Items.Add(hideItem);
            layerContextMenu.Items.Add(showItem);
        }




        /// <summary>
        /// 删除树节点名称中的显隐状态文字
        /// </summary>
        private string RemoveVisibilityText(string text)
        {
            if (string.IsNullOrEmpty(text))
                return string.Empty;

            return text.Replace(" [已隐藏]", "");
        }


        /// <summary>
        /// 获取树节点原本应该显示的颜色
        /// </summary>
        private Color GetTreeNodeOriginalColor(TreeNode node)
        {
            if (node == null)
                return Color.Black;

            // Component 节点的 Tag 中保存了 Component 原始颜色
            if (node.Tag is Color nodeColor)
                return nodeColor;

            // 面节点的 Tag 保存的是 Shape ID，
            // 所以从父 Component 的 Tag 中读取原始颜色
            if (node.Parent != null &&
                node.Parent.Tag is Color parentColor)
            {
                return parentColor;
            }

            return Color.Black;
        }


        /// <summary>
        /// 更新单个树节点的显隐文字和颜色
        /// </summary>
        private void SetTreeNodeVisibilityAppearance(
            TreeNode node,
            bool isShow)
        {
            if (node == null)
                return;

            node.Text = RemoveVisibilityText(node.Text);

            if (isShow)
            {
                node.ForeColor =
                    GetTreeNodeOriginalColor(node);
            }
            else
            {
                node.ForeColor = Color.Gray;
                node.Text += " [已隐藏]";
            }
        }


        /// <summary>
        /// 隐藏或显示 Component、图层以及单个面
        /// </summary>
        private void ToggleLayerVisibility(bool isShow)
        {
            TreeNode selectedNode =
                treeView1.SelectedNode;

            if (selectedNode == null ||
                myOCCTProxy == null)
            {
                return;
            }

            // ========================================================
            // 情况一：右键点击的是单个面节点
            //
            // 面节点的 Tag 中保存的是 newFaceId。
            // 此时只修改这一个面，不遍历整个 Component。
            // ========================================================
            if (selectedNode.Tag is int shapeId)
            {
                SetTreeNodeVisibilityAppearance(
                    selectedNode,
                    isShow);

                myOCCTProxy.SetShapeVisibility(
                    shapeId,
                    isShow);

                // 如果 Component 原来处于整体隐藏状态，
                // 现在单独显示其中一个面，则父节点不应继续显示“已隐藏”
                if (isShow &&
                    selectedNode.Parent != null)
                {
                    TreeNode parentNode =
                        selectedNode.Parent;

                    parentNode.Text =
                        RemoveVisibilityText(parentNode.Text);

                    parentNode.ForeColor =
                        GetTreeNodeOriginalColor(parentNode);
                }

                myOCCTProxy.UpdateCurrentViewer();
                return;
            }

            // ========================================================
            // 情况二：右键点击的是 Component 或普通图层
            //
            // 保留原来的整体隐藏/显示功能。
            // ========================================================
            List<int> shapeIds;

            if (!layerShapesMap.TryGetValue(
                selectedNode,
                out shapeIds))
            {
                return;
            }

            SetTreeNodeVisibilityAppearance(
                selectedNode,
                isShow);

            // 整体修改 Component 中的所有几何体
            foreach (int currentShapeId in shapeIds)
            {
                myOCCTProxy.SetShapeVisibility(
                    currentShapeId,
                    isShow);
            }

            // 同步修改 Component 下面每个面节点的字体状态
            foreach (TreeNode childNode in selectedNode.Nodes)
            {
                if (childNode.Tag is int)
                {
                    SetTreeNodeVisibilityAppearance(
                        childNode,
                        isShow);
                }
            }

            myOCCTProxy.UpdateCurrentViewer();
        }
        private void UpdateTreeAfterCombine(int oldId1, int oldId2, int newId)
        {
            foreach (var layerPair in layerShapesMap)
            {
                TreeNode layerNode = layerPair.Key;
                List<int> idsInLayer = layerPair.Value;

                // 假设这两条线在同一个图层里（常规情况）
                if (idsInLayer.Contains(oldId1) || idsInLayer.Contains(oldId2))
                {
                    // 注销旧 ID
                    idsInLayer.Remove(oldId1);
                    idsInLayer.Remove(oldId2);
                    // 登记新 ID
                    idsInLayer.Add(newId);

                    // 如果你想在左侧 UI 树状图上视觉同步，可以在这删旧节点加新节点
                    // ... (根据需要实现)
                    break;
                }
            }
        }
        /// <summary>
        /// 智能合并后，批量从图层字典中清理被消耗的碎线 ID
        /// </summary>
        private void UpdateTreeAfterAutoCombine(int[] consumedIds, int newId)
        {
            // 顺藤摸瓜：相连的线通常在同一个图层。我们只要找到起点所在的图层即可
            foreach (var layerPair in layerShapesMap)
            {
                List<int> idsInLayer = layerPair.Value;

                // 只要这个图层里包含了我们点击的第一根线
                if (idsInLayer.Contains(consumedIds[0]))
                {
                    // 批量把所有被熔接的碎线从花名册里剔除
                    foreach (int oldId in consumedIds)
                    {
                        idsInLayer.Remove(oldId);
                    }

                    // 把新诞生的巨无霸红线 ID 加进去
                    idsInLayer.Add(newId);

                    break; // 搞定，收工！
                }
            }
        }
        /// <summary>
        /// 打断成功后，更新图层字典，剔除旧 ID，塞入新碎片的 ID
        /// </summary>
        private void UpdateTreeAfterSplit(int targetOldId, int[] newSplitIds)
        {
            foreach (var layerPair in layerShapesMap)
            {
                List<int> idsInLayer = layerPair.Value;

                // 找到死去的旧线在哪个图层
                if (idsInLayer.Contains(targetOldId))
                {
                    idsInLayer.Remove(targetOldId);      // 注销旧线
                    idsInLayer.AddRange(newSplitIds);    // 登记新切碎的线段

                    break; // 收工
                }
            }
        }
        /// <summary>
        /// 删除线条后，从 C# 的图层字典中剔除该 ID
        /// </summary>
        private void UpdateTreeAfterDelete(int targetId)
        {
            foreach (var layerPair in layerShapesMap)
            {
                List<int> idsInLayer = layerPair.Value;

                if (idsInLayer.Remove(targetId))
                {
                    TreeNode nodeToRemove = null;

                    foreach (TreeNode childNode in layerPair.Key.Nodes)
                    {
                        if (childNode.Tag is int shapeId && shapeId == targetId)
                        {
                            nodeToRemove = childNode;
                            break;
                        }
                    }

                    nodeToRemove?.Remove();
                    break;
                }
            }
        }
        /// <summary>
        /// 判断节点是否是可以保存手工直线的普通图层。
        /// 同时兼容项目中已有的半角冒号“图层:”和全角冒号“图层：”。
        /// </summary>
        private bool IsLineDrawingLayerNode(TreeNode node)
        {
            if (node == null || !layerShapesMap.ContainsKey(node))
                return false;

            string nodeText = RemoveVisibilityText(node.Text);
            return nodeText.StartsWith("图层:", StringComparison.Ordinal) ||
                nodeText.StartsWith("图层：", StringComparison.Ordinal);
        }

        /// <summary>
        /// 返回不包含 Current 和隐藏状态后缀的图层名称。
        /// </summary>
        private string GetLineLayerName(TreeNode node)
        {
            if (node == null)
                return "未设置图层";

            return RemoveVisibilityText(node.Text)
                .Replace(" (Current)", "")
                .Trim();
        }

        /// <summary>
        /// 将指定普通图层设为手工直线的当前保存图层。
        /// 该状态独立于面创建使用的 Current Component。
        /// </summary>
        private void SetCurrentLineLayer(TreeNode targetNode)
        {
            if (!IsLineDrawingLayerNode(targetNode))
                return;

            // 恢复上一个当前图层的普通显示样式。
            if (currentLineLayerNode != null &&
                currentLineLayerNode != targetNode)
            {
                currentLineLayerNode.Text =
                    currentLineLayerNode.Text.Replace(" (Current)", "");
                currentLineLayerNode.NodeFont =
                    new Font(treeView1.Font, FontStyle.Regular);
            }

            currentLineLayerNode = targetNode;

            if (!currentLineLayerNode.Text.Contains(" (Current)"))
                currentLineLayerNode.Text += " (Current)";

            currentLineLayerNode.NodeFont =
                new Font(treeView1.Font, FontStyle.Bold);
            treeView1.SelectedNode = currentLineLayerNode;

            LogMessage(
                $"当前绘图图层已切换为: {GetLineLayerName(currentLineLayerNode)}。" +
                "后续手工创建的直线将保存到该图层。");
        }

        /// <summary>
        /// 拦截树状图的鼠标点击事件
        /// </summary>
        private void treeView1_NodeMouseClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            // 只有右键点击才触发菜单
            if (e.Button == MouseButtons.Right)
            {
                // 【关键修复】：强制将右键点击的节点设为当前选中节点！
                // 这样你的 ToggleLayerVisibility 方法里调用 treeView1.SelectedNode 就绝对不会错
                treeView1.SelectedNode = e.Node;

                ContextMenuStrip dynamicMenu = new ContextMenuStrip();

                var hideItem = new ToolStripMenuItem("隐藏 (Hide)");
                hideItem.Click += (s, args) => ToggleLayerVisibility(false);
                dynamicMenu.Items.Add(hideItem);

                var showItem = new ToolStripMenuItem("显示 (Show)");
                showItem.Click += (s, args) => ToggleLayerVisibility(true);
                dynamicMenu.Items.Add(showItem);

                // 任一普通图层都可以设为手工直线的当前保存图层。
                if (IsLineDrawingLayerNode(e.Node))
                {
                    dynamicMenu.Items.Add(new ToolStripSeparator());

                    var makeCurrentLayerItem =
                        new ToolStripMenuItem("设置为当前 (Make Current)");
                    makeCurrentLayerItem.Checked =
                        currentLineLayerNode == e.Node;
                    makeCurrentLayerItem.Click +=
                        (s, args) => SetCurrentLineLayer(e.Node);
                    dynamicMenu.Items.Add(makeCurrentLayerItem);
                }

                // 1. 如果右键点击的是“图纸”或“基体” (根级节点)
                if(e.Node.Level == 0)
                {
                    var createCordLayerItem = new ToolStripMenuItem("新建帘线图层 (Create Cord Layer)");
                    createCordLayerItem.Click += (s, args) =>
                    {
                        // 实例化独立图层节点并赋予标识性视觉属性
                        TreeNode cordNode = new TreeNode($"图层：帘线");
                        cordNode.ForeColor = Color.Green;

                        // 挂载至当前右击的父节点下
                        e.Node.Nodes.Add(cordNode);

                        // 在底层数据字典中初始化此层级的实体内存空间
                        layerShapesMap[cordNode] = new List<int>();

                        // 强制转移上下文焦点至新生图层，并展开目录树
                        treeView1.SelectedNode = cordNode;
                        e.Node.Expand();
                    };
                    dynamicMenu.Items.Add(createCordLayerItem);

                }
                else if (e.Node.Text.Contains("基体") )
                {
                    var createCompItem = new ToolStripMenuItem("新建组件 (Create Component)");
                    createCompItem.Click += (s, args) => CreateNewComponent(e.Node);
                    dynamicMenu.Items.Add(createCompItem);
                }

                // 2. Component 的 Current 状态继续单独用于面创建和拓扑编辑。
                else if (e.Node.Text.StartsWith("Component"))
                {
                    var makeCurrentItem =
                        new ToolStripMenuItem("设置为当前组件 (Make Current Component)");
                    makeCurrentItem.Click += (s, args) => SetCurrentComponent(e.Node);
                    dynamicMenu.Items.Add(makeCurrentItem);

                    dynamicMenu.Items.Add(new ToolStripSeparator()); 

                    
                }

                // 3. 在鼠标点击的位置弹出菜单
                if (dynamicMenu.Items.Count > 0)
                {
                    dynamicMenu.Show(treeView1, e.Location);
                }
            }
        }



        private void UpdateTreeForNewFace(int newFaceId)
        {
            string baseLayerName = "基体";
            TreeNode baseLayerNode = null;

            // 1. 在树里找找看有没有叫 "基体" 的节点
            foreach (TreeNode node in treeView1.Nodes[0].Nodes) // 假设第一层是根节点"轮胎模型"
            {
                if (node.Text == $"图层: {baseLayerName}")
                {
                    baseLayerNode = node;
                    break;
                }
            }

            // 2. 如果没有，就创建一个“基体”图层
            if (baseLayerNode == null)
            {
                baseLayerNode = new TreeNode($"图层: {baseLayerName}");
                treeView1.Nodes[0].Nodes.Add(baseLayerNode);
                layerShapesMap[baseLayerNode] = new List<int>(); // 别忘了在花名册开个户
            }

            // 3. 在“基体”如图层下挂载新的面节点
            TreeNode faceNode = new TreeNode($"面 {faceCounter}");
            baseLayerNode.Nodes.Add(faceNode);

            // 4. 将面的 ID 记在基体图层的花名册里
            layerShapesMap[baseLayerNode].Add(newFaceId);

            // 展开节点让用户看见
            baseLayerNode.Expand();
        }




    }
}
