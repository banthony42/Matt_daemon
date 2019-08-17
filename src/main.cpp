// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: banthony <banthony@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2019/08/17 12:29:26 by banthony          #+#    #+#             //
//   Updated: 2019/08/17 12:29:27 by banthony         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "Tintin_reporter.hpp"

int main()
{
    Tintin_reporter lConsole = Tintin_reporter();
    Tintin_reporter lMattDaemon = Tintin_reporter("./matt_daemon.log");

    lConsole.Log(INFO, "Started.");
    lConsole.Log(WARNING, "Test with a little warning.");
    lConsole.Log(ERROR, "Test with an error.");
    lMattDaemon.Log(INFO, "Daemon Started.");
}