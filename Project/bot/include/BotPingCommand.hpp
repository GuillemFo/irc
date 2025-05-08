/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BotPingCommand.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzhdanov <rzhdanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 21:55:39 by rzhdanov          #+#    #+#             */
/*   Updated: 2025/05/08 22:04:22 by rzhdanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOTPINGCOMMAND_HPP
# define BOTPINGCOMMAND_HPP

# include "IncomingCommand.hpp"

class PingCommand : public IncomingCommand {
	public:
		virtual bool matches(const std::string& message) const;
		virtual std::string respond(const std::string& sender) const;
};

#endif
