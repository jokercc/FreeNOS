#
# Copyright (C) 2010 Niek Linnenbank
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

import os
import tempfile
import shutil

#
# Generate a bootable ISO image.
#
def iso_func(target, source, env):

    # Create a temporary directory.
    temp = tempfile.mkdtemp()

    # Temporary workaround for intel/nuc. Place grub.cfg in /boot/grub.
    os.makedirs(temp + '/boot/grub')

    # Copy required files to temp directory.
    for s in source:
        shutil.copy(str(s), temp + '/boot')

    # Generate the ISO.
    if env.Detect('grub-mkrescue'):
        shutil.copy('kernel/intel/nuc/grub.cfg', temp + '/boot/grub')
        os.system('grub-mkrescue -d /usr/lib/grub/i386-pc -o ' + str(target[0]) + ' --modules="multiboot iso9660 biosdisk gzio" ' + temp)

    # Fallback without grub2.
    elif env.Detect('mkisofs'):
        shutil.copy('kernel/intel/nuc/menu.lst', temp + '/boot/grub')
        shutil.copy('kernel/intel/nuc/stage2_eltorito', temp)
        os.system('mkisofs -quiet -R -b stage2_eltorito -no-emul-boot ' +
                  '-boot-load-size 4 -boot-info-table -o ' + str(target[0]) +
                  ' -V "FreeNOS ' + env['VERSION'] + '" ' + temp);
    else:
        raise Exception("no ISO generation program found. Install grub-mkrescue or mkisofs")

    # Clean up temporary directory.
    shutil.rmtree(temp);

#
# String command representation for ISO builder.
#
def iso_str(target, source, env):

    return "  ISO  " + str(target[0])

#
# Add ourselves to the given environment.
#
def generate(env):

    builder = env.Builder(action  = env.Action(iso_func, iso_str))
    env.Append(BUILDERS = { 'ISO' : builder })

#
# We always exist.
#
def exists(env):
    return Env.Detect('grub-mkrescue')
