import torch.nn as nn

class Net(nn.Module):
    def __init__(self):
        super().__init__()

        self.conv1 = nn.Conv2d(3, 16, 3, padding=1)
        self.conv2 = nn.Conv2d(16, 64, 3, padding=1)
        self.conv3 = nn.Conv2d(64, 128, 3, padding=1)
        self.pool = nn.MaxPool2d(2, 2)
        self.avgpool = nn.AdaptiveAvgPool2d((8, 8))
        self.dropout = nn.Dropout(p=0.4)
        self.fc1 = nn.Linear(8192, 512)
        self.fc2 = nn.Linear(512, 128)
        self.fc3 = nn.Linear(128, 4)

        self.sequential = nn.Sequential(
               nn.Conv2d(3, 16, 11, stride=4),
               nn.MaxPool2d(3, stride=2),
               nn.Conv2d(16, 32, 11, stride=4),
               nn.MaxPool2d(3, stride=2),
               nn.Conv2d(32, 64, 3, padding=1),
               nn.ReLU(),
               nn.Conv2d(64, 128, 3, padding=1),
               nn.ReLU(),
               nn.Conv2d(128, 256, 3, padding=1),
               nn.ReLU(),
               nn.MaxPool2d(3, stride=2),
               nn.Flatten(),
               nn.Linear(1024, 512),
               nn.Dropout(0.5),
               nn.Linear(512, 256),
               nn.Dropout(0.5),
               nn.Linear(256, 4)
        )

    def forward(self, x):
        x = self.sequential(x)

        return x