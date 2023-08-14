#include <iostream>
#include "Common.h"

using namespace std;

class Rectangle : public IShape 
{
public:
	Rectangle() {}

	std::unique_ptr<IShape> Clone() const
	{
		std::unique_ptr<IShape> rectangle = std::make_unique<Rectangle>();
		rectangle->SetPosition(position_);
		rectangle->SetSize(size_);
		rectangle->SetTexture(texture_);

		return rectangle;
	}

	void SetPosition(Point p)
	{
		position_.x = p.x;
		position_.y = p.y;
	}

  	Point GetPosition() const
  	{
  		return position_;
  	}

  	void SetSize(Size s)
  	{
 		size_.width = s.width;
 		size_.height = s.height;
  	}

   	Size GetSize() const
   	{
   		return size_;
   	}

	void SetTexture(std::shared_ptr<ITexture> texture)
	{
		texture_ = texture;
	}

	ITexture* GetTexture() const
	{
		return texture_.get();
	}

	void Draw(Image& im) const
	{
		for (int y = position_.y; y < position_.y + size_.height; y++)
		{
			if (y < im.size())
			{
				for (int x = position_.x; x < position_.x + size_.width; x++)
				{
					if (x < im[0].size())
					{
						if (texture_)
						{
							if (y < position_.y + texture_->GetSize().height && x < position_.x + texture_->GetSize().width)
							{
								int sy = y - position_.y;
								int sx = x - position_.x;
								im[y][x] = texture_->GetImage()[sy][sx];
							}
							else
							{
								im[y][x] = '.';
							}
						}
						else
						{
							im[y][x] = '.';
						}
					}
				}
			}
		}
	}

private:
	Point position_;
	Size size_;
	std::shared_ptr<ITexture> texture_;
};

class Ellipse : public IShape 
{
public:
	Ellipse() {}

	std::unique_ptr<IShape> Clone() const
	{
		std::unique_ptr<IShape> ellipse = std::make_unique<Ellipse>();
		ellipse->SetPosition(position_);
		ellipse->SetSize(size_);
		ellipse->SetTexture(texture_);

		return ellipse;
	}

	void SetPosition(Point p)
	{
		position_.x = p.x;
		position_.y = p.y;
	}

  	Point GetPosition() const
  	{
  		return position_;
  	}

  	void SetSize(Size s)
  	{
 		size_.width = s.width;
 		size_.height = s.height;
  	}

   	Size GetSize() const
   	{
   		return size_;
   	}

	void SetTexture(std::shared_ptr<ITexture> texture)
	{
		texture_ = texture;
	}

	ITexture* GetTexture() const
	{
		return texture_.get();
	}

	void Draw(Image& im) const
	{
		for (int y = position_.y; y < position_.y + size_.height; y++)
		{
			if (y < im.size())
			{
				for (int x = position_.x; x < position_.x + size_.width; x++)
				{
					if (x < im[0].size())
					{
						int sy = y - position_.y;
						int sx = x - position_.x;
						if (IsPointInEllipse(Point{sx, sy}, size_))
						{
							if (texture_)
							{
								if (y < position_.y + texture_->GetSize().height && x < position_.x + texture_->GetSize().width)
								{
									im[y][x] = texture_->GetImage()[sy][sx];
								}
								else
								{
									im[y][x] = '.';
								}
							}
							else
							{
								im[y][x] = '.';
							}
						}
					}
				}
			}
		}
	}

private:
	Point position_;
	Size size_;
	std::shared_ptr<ITexture> texture_;
};

unique_ptr<IShape> MakeShape(ShapeType shape_type) {
	if (shape_type == ShapeType::Rectangle)
	{
		std::unique_ptr<IShape> rectangle = std::make_unique<Rectangle>();
		return rectangle;
	}
	std::unique_ptr<IShape> ellipse = std::make_unique<Ellipse>();
	return ellipse;
}