#include "splitter.h"
#include <tuple>

void Splitter::AppendVertex(const math::Vec2 & vertex)
{
	auto iter = std::find(_vertexs.begin(), _vertexs.end(), vertex);
	if (iter == _vertexs.end())
	{
		UpdateVertex(vertex);
	}
}

void Splitter::RemoveVertex(const math::Vec2 & vertex)
{
	auto iter = std::remove(_vertexs.begin(), _vertexs.end(), vertex);
	if (iter != _vertexs.end())
	{
		_vertexs.erase(iter);
		auto helper = std::move(_helper);
		auto vertexs = decltype(_vertexs)(
			_vertexs.begin() + 4,
			_vertexs.end());

		_triangles.clear();
		_vertexs.clear();
		_lines.clear();
		SetHelperVertex(helper);
		std::for_each(vertexs.begin(), vertexs.end(), 
			BIND_1(Splitter::AppendVertex, this));
	}
}

void Splitter::SetHelperVertex(const std::array<math::Vec2, 4> & vertexs)
{
	std::copy(vertexs.begin(), vertexs.end(), std::back_inserter(_vertexs));
	_triangles.emplace_back(vertexs.at(0), vertexs.at(1), vertexs.at(2));
	_triangles.emplace_back(vertexs.at(0), vertexs.at(2), vertexs.at(3));
	_lines.emplace_back(vertexs.at(0), vertexs.at(1));
	_lines.emplace_back(vertexs.at(0), vertexs.at(2));
	_lines.emplace_back(vertexs.at(0), vertexs.at(3));
	_lines.emplace_back(vertexs.at(2), vertexs.at(1));
	_lines.emplace_back(vertexs.at(2), vertexs.at(3));
	_helper = vertexs;
}

void Splitter::UpdateVertex(const math::Vec2 & vertex)
{
	std::vector<Triangle> commonTriangles;
	for (auto & triangle: _triangles)
	{
		auto result = math::Circumcircle(triangle.pt1, triangle.pt2, triangle.pt3);
		if (std::get<0>(result))
		{
			if (std::get<1>(result) >= (vertex - std::get<2>(result)).Length())
			{
				commonTriangles.push_back(triangle);
			}
		}
	}

	if (commonTriangles.size() == 1)
	{
		const auto & triangle = commonTriangles.at(0);
		RemoveFromVector(_triangles, triangle);
		AppendToVector(_lines, { vertex, triangle.pt1 });
		AppendToVector(_lines, { vertex, triangle.pt2 });
		AppendToVector(_lines, { vertex, triangle.pt3 });
		auto triangle1 = Triangle(vertex, triangle.pt1, triangle.pt2);
		auto triangle2 = Triangle(vertex, triangle.pt2, triangle.pt3);
		auto triangle3 = Triangle(vertex, triangle.pt3, triangle.pt1);
		if (IsExistsTriangle(triangle1))
		{
			AppendToVector(_triangles, triangle1);
		}
		if (IsExistsTriangle(triangle2))
		{
			AppendToVector(_triangles, triangle2);
		}
		if (IsExistsTriangle(triangle3))
		{
			AppendToVector(_triangles, triangle3);
		}
	}
	else if (commonTriangles.size() > 1)
	{
		Line commonLine;
		for (auto i = 0; i != commonTriangles.size() - 1; ++i)
		{
			for (auto j = i + 1; j != commonTriangles.size(); ++j)
			{
				const auto & triangle1 = commonTriangles.at(i);
				const auto & triangle2 = commonTriangles.at(j);
				if (triangle1.QueryCommonLine(triangle2, &commonLine))
				{
					RemoveFromVector(_lines, commonLine);
					RemoveFromVector(_triangles, [&commonLine](const Triangle & triangle) 
						{ 
							return triangle.IsExistsLine(commonLine);
						});

					AppendToVector(_lines, { vertex, triangle1.pt1 });
					AppendToVector(_lines, { vertex, triangle1.pt2 });
					AppendToVector(_lines, { vertex, triangle1.pt3 });
					AppendToVector(_lines, { vertex, triangle2.pt1 });
					AppendToVector(_lines, { vertex, triangle2.pt2 });
					AppendToVector(_lines, { vertex, triangle2.pt3 });

					auto newTriangle1 = Triangle(vertex, triangle1.pt1, triangle1.pt2);
					auto newTriangle2 = Triangle(vertex, triangle1.pt2, triangle1.pt3);
					auto newTriangle3 = Triangle(vertex, triangle1.pt3, triangle1.pt1);
					auto newTriangle4 = Triangle(vertex, triangle2.pt1, triangle2.pt2);
					auto newTriangle5 = Triangle(vertex, triangle2.pt2, triangle2.pt3);
					auto newTriangle6 = Triangle(vertex, triangle2.pt3, triangle2.pt1);
					if (IsExistsTriangle(newTriangle1)) { AppendToVector(_triangles, newTriangle1); }
					if (IsExistsTriangle(newTriangle2)) { AppendToVector(_triangles, newTriangle2); }
					if (IsExistsTriangle(newTriangle3)) { AppendToVector(_triangles, newTriangle3); }
					if (IsExistsTriangle(newTriangle4)) { AppendToVector(_triangles, newTriangle4); }
					if (IsExistsTriangle(newTriangle5)) { AppendToVector(_triangles, newTriangle5); }
					if (IsExistsTriangle(newTriangle6)) { AppendToVector(_triangles, newTriangle6); }
				}
			}
		}
	}
	AppendToVector(_vertexs, vertex);
}

bool Splitter::IsExistsTriangle(const Triangle & triangle)
{
	auto line1 = Line(triangle.pt1, triangle.pt2);
	auto line2 = Line(triangle.pt2, triangle.pt3);
	auto line3 = Line(triangle.pt3, triangle.pt1);
	auto count = 0;
	for (const auto & line : _lines)
	{
		if (line1 == line) count |= 0x1;
		if (line2 == line) count |= 0x2;
		if (line3 == line) count |= 0x4;
	}
	return count == (0x1 | 0x2 | 0x4);
}

Splitter::Triangle::Triangle(const math::Vec2 & _pt1, const math::Vec2 & _pt2, const math::Vec2 & _pt3)
	: pt1(_pt1)
	, pt2(_pt2)
	, pt3(_pt3)
{ }

bool Splitter::Triangle::operator==(const Triangle & other) const
{
	return pt1 == other.pt1 && pt2 == other.pt2 && pt3 == other.pt3
		|| pt1 == other.pt1 && pt2 == other.pt3 && pt3 == other.pt2
		|| pt1 == other.pt2 && pt2 == other.pt1 && pt3 == other.pt3
		|| pt1 == other.pt2 && pt2 == other.pt3 && pt3 == other.pt1
		|| pt1 == other.pt3 && pt2 == other.pt1 && pt3 == other.pt2
		|| pt1 == other.pt3 && pt2 == other.pt2 && pt3 == other.pt1;
}

bool Splitter::Triangle::operator!=(const Triangle & other) const
{
	return !(*this == other);
}

bool Splitter::Triangle::IsExistsLine(const Line & line) const
{
	return Line(pt1, pt2) == line
		|| Line(pt2, pt3) == line
		|| Line(pt3, pt1) == line;
}

bool Splitter::Triangle::QueryCommonLine(const Triangle & triangle, Line * out) const
{
	std::vector<Line> lines1{
		{ pt1, pt2 },{ pt2, pt3 },{ pt3, pt1 }
	};
	std::vector<Line> lines2{
		{ triangle.pt1, triangle.pt2 },
		{ triangle.pt2, triangle.pt3 },
		{ triangle.pt3, triangle.pt1 }
	};
	for (const auto & line1 : lines1)
	{
		for (const auto & line2 : lines2)
		{
			if (line1 == line2) { *out = line1; return true; }
		}
	}
	return false;
}

Splitter::Line::Line(const math::Vec2 & _pt1, const math::Vec2 & _pt2)
	: pt1(_pt1)
	, pt2(_pt2)
{ }

bool Splitter::Line::operator==(const Line & other) const
{
	return pt1 == other.pt1 && pt2 == other.pt2
		|| pt1 == other.pt2 && pt2 == other.pt1;
}

bool Splitter::Line::operator!=(const Line & other) const
{
	return !(*this == other);
}