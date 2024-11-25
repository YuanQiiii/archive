import torch
import torch.nn as nn
import torch.nn.functional as F

class SimpleBlock(nn.Module):
    def __init__(self, in_channel, out_channel, stride=1):
        super(SimpleBlock, self).__init__()
        self.conv = nn.Conv2d(in_channel, out_channel, kernel_size=3, stride=stride, padding=1, bias=False)
        self.bn = nn.BatchNorm2d(out_channel)

    def forward(self, x):
        x = F.relu(self.bn(self.conv(x)), inplace=True)
        return x

class SimpleResNet(nn.Module):
    def __init__(self):
        super(SimpleResNet, self).__init__()
        self.prepare = nn.Sequential(
            nn.Conv2d(133, 64, 3, 1, 1, bias=False),
            nn.BatchNorm2d(64),
            nn.ReLU(inplace=True),
            nn.MaxPool2d(2, 2)
        )
        self.layer1 = self._make_layer(64, 128, 2, 1)
        self.layer2 = self._make_layer(128, 256, 2, 2)
        self.layer3 = self._make_layer(256, 512, 2, 2)
        self.pool = nn.AdaptiveAvgPool2d((1, 1))
        self.fc = nn.Sequential(
            nn.Linear(512, 256),
            nn.ReLU(inplace=True),
            nn.Linear(256, 235)
        )
        self._tower = nn.Sequential(
            self.prepare,
            self.layer1,
            self.layer2,
            self.layer3,
            self.pool,
            nn.Flatten(),
            self.fc
        )
        for m in self.modules():
            if isinstance(m, nn.Conv2d) or isinstance(m, nn.Linear):
                nn.init.kaiming_normal_(m.weight)

    def _make_layer(self, in_channel, out_channel, blocks, stride):
        layers = [SimpleBlock(in_channel, out_channel, stride)]
        for _ in range(1, blocks):
            layers.append(SimpleBlock(out_channel, out_channel))
        return nn.Sequential(*layers)

    def forward(self, input_dict):
        self.train(mode=input_dict.get("is_training", False))
        obs = input_dict["obs"]["observation"].float()

        if obs.dim() == 3:
            obs = obs.unsqueeze(0)

        action_logits = self._tower(obs)
        action_mask = input_dict["obs"]["action_mask"].float()
        inf_mask = torch.clamp(torch.log(action_mask), -1e38, 1e38)
        return action_logits + inf_mask

# 使用示例
# model = SimpleResNet()
