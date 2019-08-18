// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Utils.hpp                                          :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: abara <banthony@student.42.fr>             +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2019/08/18 13:33:18 by abara             #+#    #+#             //
//   Updated: 2019/08/18 13:33:19 by abara            ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <string>

class Utils
{
    public:

    Utils();                        // Coplien, Canonical
    Utils(Utils &iCopy);            // Coplien, Canonical
    ~Utils();                       // Coplien, Canonical
    Utils &operator=(Utils &iCopy); // Coplien, Canonical

    static bool FileExist(const std::string &iPath);

    private:
};