// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Utils.cpp                                          :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: abara <banthony@student.42.fr>             +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2019/08/18 13:33:02 by abara             #+#    #+#             //
//   Updated: 2019/08/18 13:33:09 by abara            ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "Utils.hpp"
#include <sys/stat.h>

bool Utils::FileExist(const std::string &iPath)
{
  struct stat lBuffer;   
  return (stat (iPath.c_str(), &lBuffer) == 0); 
}

Utils::Utils()
{
    return;   
}

Utils::Utils(Utils &iCopy)
{
    *this = iCopy;
}

Utils::~Utils()
{
    return ;
}

Utils &Utils::operator=(Utils &iCopy)
{
    if (this != &iCopy) {
        *this = iCopy;
    }
    return *this;
}