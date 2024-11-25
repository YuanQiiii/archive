from dataset import MahjongGBDataset
from torch.utils.data import DataLoader
import model
import torch.nn.functional as F
import torch
import os
from torch.utils.tensorboard import SummaryWriter

if __name__ == "__main__":
    logdir = "log/"
    os.makedirs(logdir + "checkpoint", exist_ok=True)
    writer = SummaryWriter(log_dir=logdir)

    # Load dataset
    splitRatio = 0.9
    batchSize = 1024
    trainDataset = MahjongGBDataset(0, splitRatio, True)
    validateDataset = MahjongGBDataset(splitRatio, 1, False)
    loader = DataLoader(dataset=trainDataset, batch_size=batchSize, shuffle=True)
    vloader = DataLoader(dataset=validateDataset, batch_size=batchSize, shuffle=False)

    # Load model
    model = model.SimpleResNet().to("cuda")
    optimizer = torch.optim.Adam(model.parameters(), lr=5e-4)

    # Load from checkpoint if exists
    checkpoint_path = os.path.join(logdir, "checkpoint", "last_checkpoint.pth")
    start_epoch = 0
    if os.path.isfile(checkpoint_path):
        checkpoint = torch.load(checkpoint_path)
        model.load_state_dict(checkpoint['model_state_dict'])
        optimizer.load_state_dict(checkpoint['optimizer_state_dict'])
        start_epoch = checkpoint['epoch'] + 1
        print(f"Loaded checkpoint from epoch {start_epoch}")

    # Train and validate
    num_epochs = 32
    for e in range(start_epoch, num_epochs):
        print("Epoch", e)
        # Save model with optimizer state
        torch.save({
            'epoch': e,
            'model_state_dict': model.state_dict(),
            'optimizer_state_dict': optimizer.state_dict(),
        }, checkpoint_path)
        model.train()
        for i, d in enumerate(loader):
            input_dict = {
                "is_training": True,
                "obs": {"observation": d[0].cuda(), "action_mask": d[1].cuda()},
            }
            logits = model(input_dict)
            loss = F.cross_entropy(logits, d[2].long().cuda())
            if i % 128 == 0:
                print(
                    "Iteration %d/%d" % (i, len(trainDataset) // batchSize + 1),
                    "policy_loss",
                    loss.item(),
                )
                writer.add_scalar('Loss/train', loss.item(), e * len(loader) + i)
            optimizer.zero_grad()
            loss.backward()
            optimizer.step()
        
        print("Run validation:")
        model.eval()
        correct = 0
        for i, d in enumerate(vloader):
            input_dict = {
                "is_training": False,
                "obs": {"observation": d[0].cuda(), "action_mask": d[1].cuda()},
            }
            with torch.no_grad():
                logits = model(input_dict)
                pred = logits.argmax(dim=1)
                correct += torch.eq(pred, d[2].cuda()).sum().item()
        acc = correct / len(validateDataset)
        print("Epoch", e + 1, "Validate acc:", acc)
        writer.add_scalar('Accuracy/validate', acc, e)
    
    writer.close()
