using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using netDxf;

namespace IO
{
    public static class DataInteraction
    {
        public static DxfDocument LoadDocument(string filePath, out string errorMessage)
        {
            errorMessage = string.Empty;
            try
            {
                // 使用 netDxf 核心引擎加载文件
                DxfDocument dxf = DxfDocument.Load(filePath);

                if (dxf == null)
                {
                    errorMessage = "DXF 文件解析失败或格式不兼容！";
                    return null;
                }

                return dxf;
            }
            catch (Exception ex)
            {
                errorMessage = $"读取图纸时发生致命异常: {ex.Message}";
                return null;
            }
        }









    }
}
