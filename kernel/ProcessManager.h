/*
 * Copyright (C) 2015 Niek Linnenbank
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __KERNEL_PROCESS_MANAGER_H
#define __KERNEL_PROCESS_MANAGER_H
#ifndef __ASSEMBLER__

#include <Types.h>
#include "Process.h"
#include "ProcessFactory.h"
#include "ProcessScheduler.h"

/** Maximum number of processes. */
#define MAX_PROCS 1024

/**
 * Represents a process which may run on the host.
 */
class ProcessManager
{
    public:
    
        /**
         * Constructor function.
         * @param entry Initial program counter value.
         */
        ProcessManager(ProcessFactory *factory,
                       ProcessScheduler *scheduler);
    
        /**
         * Destructor function.
         */
        virtual ~ProcessManager();

        /**
         * Create and add a new Process.
         */
        Process * create(Address entry);

        /**
         * Retrieve a Process by it's ID.
         * @param id ProcessID number.
         * @return Pointer to the appropriate process or ZERO if not found.
         */
        Process * get(ProcessID id);        

        /**
         * Remove a Process.
         */
        void remove(Process *proc);

        /**
         * Schedule next process to run.
         */
        void schedule(Process *proc = ZERO);

        /**
         * Set the idle process.
         */
        void setIdle(Process *proc);

        /**
         * Current process running. NULL if no process running yet.
         */
        Process * current();

        /**
         * Previous process running.
         */
        Process * previous();

        /**
         * Retrieve the process table.
         * @return Pointer to the process table.
         */
        Vector<Process *> * getProcessTable();

    private:

        /** All known Processes. */
        Vector<Process *> m_procs;

        /** Object which creates processes for us. */
        ProcessFactory *m_factory;

        /** Object which selects processes to run. */
        ProcessScheduler *m_scheduler;

        /** Currently executing process */
        Process *m_current;

        /** Previous process executing */
        Process *m_previous;

        /** Idle process */
        Process *m_idle;
};

/**
 * @}
 */

#endif /* __ASSEMBLER__ */
#endif /* __KERNEL_PROCESS_H */
