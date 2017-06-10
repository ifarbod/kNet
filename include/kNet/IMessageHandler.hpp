// IMessageHandler.h
//
// Author(s):       kNet Authors <https://github.com/juj/kNet>
//                  iFarbod <>
//
// Copyright (c) 2015-2017 Project CTNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

/** @file IMessageHandler.h
    @brief The \ref kNet::IMessageHandler IMessageHandler interface. Implementable by the client application. */

#include "Types.hpp"

namespace kNet
{

class MessageConnection;

/// IMessageHandler is a callback object used by the MessageConnection to invoke the main application
/// whenever a message has been received.
class IMessageHandler
{
public:
    virtual ~IMessageHandler() {}

    /// Called whenever the network stack has received a message that the application
    /// needs to process.
    /// @param source The kNet connection this message originates from.
    /// @param packetId A unique incrementing id counter that identifies the number of the UDP packet this message
    /// originated from. Use this
    ///     to prune out-of-order messages if necessary. kNet automatically discards duplicate messages, and can do
    ///     out-of-order discarding automatically as well, if you use message content ID's. Otherwise, you can use the
    ///     packetId to do the pruning manually.
    /// @param messageId Contains the id (or the "type") of the message. This is the one you specified when sending the
    /// message
    /// @param data Points to the raw data buffer. This buffer may be zero if numBytes == 0.
    /// @param numBytes The length of the raw data buffer, in bytes.
    virtual void HandleMessage(
        MessageConnection* source, packet_id_t packetId, message_id_t messageId, const char* data, size_t numBytes) = 0;

    /// Called by the network library to ask the application to produce a content ID
    /// associated with the given message. If the application returns 0, the message doesn't
    /// have a ContentID and it is processed normally.
    /// The ContentID of the message is used to determine if a message replaces another.
    virtual u32 ComputeContentID(message_id_t UNUSED(messageId), const char* UNUSED(data), size_t UNUSED(numBytes))
    {
        // The default behavior is to not have a content ID on any message.
        return 0;
    }
};

}  // ~kNet
