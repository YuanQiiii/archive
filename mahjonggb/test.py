import model
import torch


    # 确保 SimpleResNet 已在 model 模块中定义
model_instance = model.SimpleResNet()
    # 指定你的.pth文件路径
model_path  = 'data/b.pth'
    # 加载完整的checkpoint
checkpoint = torch.load(model_path, map_location=torch.device('cpu'))
    # 提取模型参数部分
model_state_dict = checkpoint['model_state_dict']


try:
        # 加载模型参数（仅模型）
    model_instance.load_state_dict(model_state_dict)
    print("Model loaded successfully")
except Exception as e:
    print(f"Error loading model: {e}")

# 设置模型为评估模式
model_instance.eval()

# 打印模型架构以确保正确加载
print(model_instance)

  