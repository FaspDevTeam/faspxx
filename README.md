Fast Auxiliary Space Preconditioning (FASP) in Cxx: README

Last modified on 08/26/2019

================================================================================

This software is free software distributed under the Lesser General Public
License or LGPL, version 3.0 or any later versions. This software distributed
in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with FASPxx. If not, see <http://www.gnu.org/licenses/>.

The FASPxx is a C++ package designed for developing parallel iterative solvers
and preconditioners for PDEs and systems of PDEs. The main components of the
package are standard Krylov methods, algebraic multigrid methods, Schwarz
methods, and ILU methods.

There is a top level cmake for configuration and building of the FASPxx shared
library and the test programs suite. You can use a straightforward cmake-style
way to compile the package. For example, the typical command to configure is:

    $ mkdir Debug; cd Debug; cmake ..   # configuration using cmake
    $ make                              # make the library as well as examples
    $ make install                      # make the library and install it