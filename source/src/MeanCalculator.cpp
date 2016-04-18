#include "MeanCalculator.hpp"

void MeanCalculator::Update()
{
  std::unique_lock<std::mutex> _lock(_mutex);
  getCurrentFrame();
  //
  // double sum = 0;
  //
  // for(int i = 0; i < n_rows; i++)
	// {
	// 	for(int j = 0; j < n_columns; j++)
	// 	{
	// 		sum += (*_frame).at<double>(i, j);
	// 	}
	// }
  //
  // _currentMean = sum/(i*j);
  //
	// std::cout << "MeanCalculator: " << _currentMean << std::endl;

}
