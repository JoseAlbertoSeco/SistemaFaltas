-- phpMyAdmin SQL Dump
-- version 5.1.1
-- https://www.phpmyadmin.net/
--
-- Servidor: 127.0.0.1
-- Tiempo de generación: 02-01-2022 a las 10:54:58
-- Versión del servidor: 10.4.20-MariaDB
-- Versión de PHP: 8.0.9

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Base de datos: `sistemaFaltas`
--

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `alumno`
--

CREATE TABLE `alumno` (
  `A_idAlumno` int(10) NOT NULL,
  `A_nombre` varchar(30) NOT NULL,
  `A_ape1` varchar(30) NOT NULL,
  `A_ape2` varchar(30) NOT NULL,
  `A_idCurso` int(10) NOT NULL,
  `A_huellaTemplate` text NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `asignatura`
--

CREATE TABLE `asignatura` (
  `AS_idAsignatura` int(10) NOT NULL,
  `AS_nombre` varchar(30) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `asig_clase`
--

CREATE TABLE `asig_clase` (
  `AC_idAsigClase` int(10) NOT NULL,
  `AC_idAsignatura` int(10) NOT NULL,
  `AC_idClase` int(10) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `clase`
--

CREATE TABLE `clase` (
  `CL_idClase` int(10) NOT NULL,
  `CL_nombre` varchar(30) NOT NULL,
  `edificio` varchar(30) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `curso`
--

CREATE TABLE `curso` (
  `C_idCurso` int(10) NOT NULL,
  `C_nombre` varchar(10) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `faltas`
--

CREATE TABLE `faltas` (
  `F_idFalta` int(10) NOT NULL,
  `F_idAlumno` int(10) NOT NULL,
  `F_fecha` date NOT NULL,
  `F_hora` int(1) NOT NULL,
  `F_idAsignatura` int(10) NOT NULL,
  `F_asistencia` char(1) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `horario`
--

CREATE TABLE `horario` (
  `H_idHorario` int(10) NOT NULL,
  `H_dia` char(1) NOT NULL,
  `H_idCurso` int(10) NOT NULL,
  `H_idAsigClase1` int(10) NOT NULL,
  `H_idAsigClase2` int(10) NOT NULL,
  `H_idAsigClase3` int(10) NOT NULL,
  `H_idAsigClase4` int(10) NOT NULL,
  `H_idAsigClase5` int(10) NOT NULL,
  `H_idAsigClase6` int(10) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Índices para tablas volcadas
--

--
-- Indices de la tabla `alumno`
--
ALTER TABLE `alumno`
  ADD PRIMARY KEY (`A_idAlumno`),
  ADD KEY `A_idCurso` (`A_idCurso`);

--
-- Indices de la tabla `asignatura`
--
ALTER TABLE `asignatura`
  ADD PRIMARY KEY (`AS_idAsignatura`);

--
-- Indices de la tabla `asig_clase`
--
ALTER TABLE `asig_clase`
  ADD PRIMARY KEY (`AC_idAsigClase`) USING BTREE,
  ADD KEY `AC_idAsignatura` (`AC_idAsignatura`),
  ADD KEY `AC_idClase` (`AC_idClase`);

--
-- Indices de la tabla `clase`
--
ALTER TABLE `clase`
  ADD PRIMARY KEY (`CL_idClase`);

--
-- Indices de la tabla `curso`
--
ALTER TABLE `curso`
  ADD PRIMARY KEY (`C_idCurso`);

--
-- Indices de la tabla `faltas`
--
ALTER TABLE `faltas`
  ADD PRIMARY KEY (`F_idFalta`,`F_idAlumno`,`F_fecha`,`F_hora`),
  ADD KEY `F_idAsignatura` (`F_idAsignatura`),
  ADD KEY `F_idAlumno` (`F_idAlumno`);

--
-- Indices de la tabla `horario`
--
ALTER TABLE `horario`
  ADD PRIMARY KEY (`H_idHorario`,`H_dia`,`H_idCurso`),
  ADD KEY `H_idCurso` (`H_idCurso`),
  ADD KEY `H_idAsigClase3` (`H_idAsigClase3`),
  ADD KEY `H_idAsigClase4` (`H_idAsigClase4`),
  ADD KEY `H_idAsigClase5` (`H_idAsigClase5`),
  ADD KEY `H_idAsigClase6` (`H_idAsigClase6`),
  ADD KEY `H_idAsigClase1` (`H_idAsigClase1`),
  ADD KEY `H_idAsigClase2` (`H_idAsigClase2`);

--
-- AUTO_INCREMENT de las tablas volcadas
--

--
-- AUTO_INCREMENT de la tabla `alumno`
--
ALTER TABLE `alumno`
  MODIFY `A_idAlumno` int(10) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT de la tabla `asignatura`
--
ALTER TABLE `asignatura`
  MODIFY `AS_idAsignatura` int(10) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT de la tabla `asig_clase`
--
ALTER TABLE `asig_clase`
  MODIFY `AC_idAsigClase` int(10) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT de la tabla `clase`
--
ALTER TABLE `clase`
  MODIFY `CL_idClase` int(10) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT de la tabla `curso`
--
ALTER TABLE `curso`
  MODIFY `C_idCurso` int(10) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT de la tabla `faltas`
--
ALTER TABLE `faltas`
  MODIFY `F_idFalta` int(10) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT de la tabla `horario`
--
ALTER TABLE `horario`
  MODIFY `H_idHorario` int(10) NOT NULL AUTO_INCREMENT;

--
-- Restricciones para tablas volcadas
--

--
-- Filtros para la tabla `alumno`
--
ALTER TABLE `alumno`
  ADD CONSTRAINT `alumno_ibfk_1` FOREIGN KEY (`A_idCurso`) REFERENCES `curso` (`C_idCurso`) ON UPDATE CASCADE;

--
-- Filtros para la tabla `asig_clase`
--
ALTER TABLE `asig_clase`
  ADD CONSTRAINT `asig_clase_ibfk_1` FOREIGN KEY (`AC_idClase`) REFERENCES `clase` (`CL_idClase`) ON UPDATE CASCADE,
  ADD CONSTRAINT `asig_clase_ibfk_2` FOREIGN KEY (`AC_idAsignatura`) REFERENCES `asignatura` (`AS_idAsignatura`) ON UPDATE CASCADE;

--
-- Filtros para la tabla `faltas`
--
ALTER TABLE `faltas`
  ADD CONSTRAINT `faltas_ibfk_1` FOREIGN KEY (`F_idAsignatura`) REFERENCES `asignatura` (`AS_idAsignatura`) ON UPDATE CASCADE,
  ADD CONSTRAINT `faltas_ibfk_2` FOREIGN KEY (`F_idAlumno`) REFERENCES `alumno` (`A_idAlumno`) ON UPDATE CASCADE;

--
-- Filtros para la tabla `horario`
--
ALTER TABLE `horario`
  ADD CONSTRAINT `horario_ibfk_1` FOREIGN KEY (`H_idCurso`) REFERENCES `curso` (`C_idCurso`) ON UPDATE CASCADE,
  ADD CONSTRAINT `horario_ibfk_2` FOREIGN KEY (`H_idAsigClase1`) REFERENCES `asig_clase` (`AC_idAsigClase`) ON UPDATE CASCADE,
  ADD CONSTRAINT `horario_ibfk_3` FOREIGN KEY (`H_idAsigClase3`) REFERENCES `asig_clase` (`AC_idAsigClase`) ON UPDATE CASCADE,
  ADD CONSTRAINT `horario_ibfk_4` FOREIGN KEY (`H_idAsigClase4`) REFERENCES `asig_clase` (`AC_idAsigClase`) ON UPDATE CASCADE,
  ADD CONSTRAINT `horario_ibfk_5` FOREIGN KEY (`H_idAsigClase5`) REFERENCES `asig_clase` (`AC_idAsigClase`) ON UPDATE CASCADE,
  ADD CONSTRAINT `horario_ibfk_6` FOREIGN KEY (`H_idAsigClase6`) REFERENCES `asig_clase` (`AC_idAsigClase`) ON UPDATE CASCADE,
  ADD CONSTRAINT `horario_ibfk_7` FOREIGN KEY (`H_idAsigClase2`) REFERENCES `asig_clase` (`AC_idAsigClase`) ON UPDATE CASCADE;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
