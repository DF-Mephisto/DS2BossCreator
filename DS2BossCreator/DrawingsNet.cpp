#include "DrawingsNet.h"

DrawingsNet::DrawingsNet(QWidget *parent)
	: blockSize(40), maxBullets(250), QWidget(parent)
{
	bullets = nullptr;
	xCount = 0;
	yCount = 0;
	layer = 0;
	layers = 0;

	setBlockCount(1, 1, 1);
}

DrawingsNet::~DrawingsNet()
{

}

bool*** DrawingsNet::getBullets()
{
	return bullets;
}

void DrawingsNet::setBlockCount(int xCount, int yCount, int layers)
{
	if (xCount <= 0 || yCount <= 0 || layers <= 0)
		return;

	for (int i = 0; i < this->layers; i++)
	{
		for (int j = 0; j < this->yCount; j++)
			delete[] bullets[i][j];

		delete[] bullets[i];
	}
	delete[] bullets;

	bullets = new bool**[layers];
	for (int i = 0; i < layers; i++)
	{
		bullets[i] = new bool*[yCount];

		for (int j = 0; j < yCount; j++)
			bullets[i][j] = new bool[xCount] { false };
	}
	
	this->xCount = xCount;
	this->yCount = yCount;
	this->layers = layers;
	layer = 0;

	//curBlockSize = (double)blockSize * ((double)(QGuiApplication::primaryScreen()->geometry().height()) / (double)1080);
	curBlockSize = blockSize;
	setFixedSize(xCount * curBlockSize + 1, yCount * curBlockSize + 1);

	bulletsLeft = maxBullets;
	emit changedBulletsLeft(bulletsLeft);

	repaint();
}

void DrawingsNet::paintEvent(QPaintEvent* e)
{
	QWidget::paintEvent(e);

	int xPos = 0;
	int yPos = 0;
	QRect border(xPos, yPos, xCount * curBlockSize, yCount * curBlockSize);

	QPainter p;
	p.begin(this);

	p.setPen(QPen(QColor("#5A5A5A")));
	p.drawRect(border);

	for (int i = 0; i < xCount - 1; i++)
	{
		xPos += curBlockSize;
		p.drawLine(xPos, yPos, xPos, border.height());
	}

	xPos = 0;
	yPos = 0;
	for (int i = 0; i < yCount - 1; i++)
	{
		yPos += curBlockSize;
		p.drawLine(xPos, yPos, border.width(), yPos);
	}

	QPixmap light = QPixmap(":/DS2BossCreator/light").scaled(curBlockSize, curBlockSize);

	for (int i = 0; i < yCount; i++)
		for (int j = 0; j < xCount; j++)
			if (bullets[layer][i][j])
				p.drawPixmap(j * curBlockSize, i * curBlockSize, light);

	p.end();
}

void DrawingsNet::mousePressEvent(QMouseEvent* e)
{
	int xCell = e->x() / curBlockSize;
	int yCell = e->y() / curBlockSize;

	if (xCell < xCount && yCell < yCount)
	{
		bool res = !bullets[layer][yCell][xCell];

		if (res && bulletsLeft)
		{
			bullets[layer][yCell][xCell] = res;
			bulletsLeft--;
		}
		else if (!res)
		{
			bullets[layer][yCell][xCell] = res;
			bulletsLeft++;
		}

		emit changedBulletsLeft(bulletsLeft);

		repaint();
	}
}

void DrawingsNet::setCurrentLayer(int layer)
{
	this->layer = layer;
	repaint();
}

int DrawingsNet::getTotalBullets()
{
	return maxBullets - bulletsLeft;
}

int DrawingsNet::getXCount()
{
	return xCount;
}

int DrawingsNet::getYCount()
{
	return yCount;
}

int DrawingsNet::getLayers()
{
	return layers;
}