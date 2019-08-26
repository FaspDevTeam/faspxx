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

The FASPxx package is designed for developing and testing new efficient solvers
and preconditioners for discrete partial differential equations (PDEs) or
systems of PDEs. The main components of the package are standard Krylov methods,
algebraic multigrid methods, and incomplete factorization methods. Based on
these standard techniques, we build efficient solvers, based on the framework
of Auxiliary Space Preconditioning, for several complicated applications.
Current examples include the fluid dynamics, underground water simulation,
the black oil model in reservoir simulation, and so on.

There is a top level Makefile which calls cmake for configuration and building
of the FASP shared library and the test programs suite.

The command to configure is:

    $ make             // to compile the FASPxx static or shared library