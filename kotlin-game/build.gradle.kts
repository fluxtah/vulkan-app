plugins {
    kotlin("multiplatform") version "1.9.21"
}

group = "com.fluxtah"
version = "1.0-SNAPSHOT"

repositories {
    mavenCentral()
}

kotlin {
    val hostOs = System.getProperty("os.name")
    val isArm64 = System.getProperty("os.arch") == "aarch64"
    val isMingwX64 = hostOs.startsWith("Windows")
    val nativeTarget = when {
        hostOs == "Mac OS X" && isArm64 -> macosArm64("native")
        hostOs == "Mac OS X" && !isArm64 -> macosX64("native")
        hostOs == "Linux" && isArm64 -> linuxArm64("native")
        hostOs == "Linux" && !isArm64 -> linuxX64("native")
        isMingwX64 -> mingwX64("native")
        else -> throw GradleException("Host OS is not supported in Kotlin/Native.")
    }

    nativeTarget.apply {
        binaries {
            sharedLib {
            }
        }
        compilations.getByName("main") {
            val modelInterop by cinterops.creating {
                defFile(project.file("cinterop/model.def"))
                packageName("com.fluxtah.application.api.interop.model")
                compilerOpts("-Icinterop")
                includeDirs("cinterop")
            }
        }
    }

    sourceSets {
        val nativeMain by getting {
            kotlin.srcDir("src/nativeMain/kotlin")
        }
        val nativeTest by getting
    }

    tasks.withType<Wrapper> {
        gradleVersion = "8.1.1"
        distributionType = Wrapper.DistributionType.BIN
    }
}