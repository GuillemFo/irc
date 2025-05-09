/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IncomingCommand.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rzhdanov <rzhdanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 21:36:23 by rzhdanov          #+#    #+#             */
/*   Updated: 2025/05/09 00:32:56 by rzhdanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCOMINGCOMMAND_HPP
# define INCOMINGCOMMAND_HPP

# include <string>

class IncomingCommand {
	public:
		virtual ~IncomingCommand() {}
		virtual bool matches(const std::string& message) const = 0;
		virtual std::string respond(const std::string& sender) const = 0;
};

#endif
