"use client";
import { useRouter } from "next/navigation";
import { SyntheticEvent, useState } from "react";
import Image from "next/image";
import { useAuth } from "@/context/authContext";
import toast from "react-hot-toast";
import LoadingPage from "@/components/loaders/LoadingPage";
import { Label } from "@/components/ui/label";
import { Input } from "@/components/ui/input";
import { Button } from "@/components/ui/button";

export default function Login() {
  const { signIn, isLoading } = useAuth();
  const [error, setError] = useState<string | null>("");
  const [imageLoading, setImageLoading] = useState<boolean>(true);
  const [showPassword, setShowPassword] = useState(false);
  const router = useRouter();

  async function handleSubmit(event: SyntheticEvent) {
    event.preventDefault();
    toast.remove();
    const response = await toast.promise(signIn(event), {
      loading: "Entrando...",
      success: () => {
        router.replace("/");
        return "Login realizado com sucesso";
      },
      error: (error) => {
        setError(error.message);
        return `${error.message}`; 
      },
    });
  }

  return !isLoading ? (
    <div
      style={{
        minHeight: "100vh",
        background: "linear-gradient(to top, #FCF191 0%, #FACC15 100%)",
      }}
      className="grid w-full grid-cols-1 lg:grid-cols-2"
    >
      <div className="relative hidden lg:flex items-center justify-center bg-transparent">
        <Image
          src="/colmeia.png"
          alt="Colmeia"
          width={400}
          height={400}
          className="object-contain z-10"
        />
        <div className="absolute inset-0 bg-gradient-to-r from-gray-900/50 to-transparent" />
      </div>
      <div className="flex items-center justify-center p-6 lg:p-12">
        <div
          className="w-full max-w-md space-y-6 shadow-lg rounded-2xl p-8"
          style={{ background: "#FFE568" }}
        >

          <div className="flex flex-col items-center space-y-2 text-center">
            <div className="pb-5"></div>
            <h1 className="text-3xl text-black font-bold">Bem vindo de volta!</h1>
          </div>
          <form
            className="space-y-4"
            onSubmit={handleSubmit}
            onChange={() => setError("")}
          >
            <div className="space-y-2 text-black">
              <Label htmlFor="email" className={error ? "text-red-500" : ""}>
                Email
              </Label>
              <Input
                id="email"
                placeholder="m@example.com"
                required
                type="email"
                className={error ? "border-spacing-5 border-red-500 bg-white text-black" : "bg-white text-black"}
              />
            </div>
            <div className="space-y-2 text-black">
              <div className="flex items-center justify-between">
                <Label
                  htmlFor="password"
                  className={error ? "text-red-500" : ""}
                >
                  Senha
                </Label>
              </div>
              <div className="relative">
                <Input
                  id="password"
                  required
                  type={showPassword ? "text" : "password"}
                  className={error ? "border-spacing-5 border-red-500 bg-white text-black pr-10" : "bg-white text-black pr-10"}
                />
                <button
                  type="button"
                  onClick={() => setShowPassword((prev) => !prev)}
                  className="absolute inset-y-0 right-2 flex items-center px-2 focus:outline-none"
                  tabIndex={-1}
                  aria-label={showPassword ? "Ocultar senha" : "Mostrar senha"}
                >
                  {showPassword ? (
                    <svg xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" strokeWidth={1.5} stroke="currentColor" className="w-5 h-5 text-gray-600">
                      <path strokeLinecap="round" strokeLinejoin="round" d="M3.98 8.223A10.477 10.477 0 0 0 2.001 12c2.25 4.5 6.75 7.5 12 7.5 2.042 0 3.98-.417 5.73-1.177M15 12a3 3 0 1 1-6 0 3 3 0 0 1 6 0Zm6.02-3.777A10.477 10.477 0 0 1 21.999 12c-2.25 4.5-6.75 7.5-12 7.5a10.477 10.477 0 0 1-5.73-1.177" />
                      <path strokeLinecap="round" strokeLinejoin="round" d="M3 3l18 18" />
                    </svg>
                  ) : (
                    <svg xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" strokeWidth={1.5} stroke="currentColor" className="w-5 h-5 text-gray-600">
                      <path strokeLinecap="round" strokeLinejoin="round" d="M2.25 12c0-4.5 6-7.5 9.75-7.5S21.75 7.5 21.75 12c0 4.5-6 7.5-9.75 7.5S2.25 16.5 2.25 12Z" />
                      <path strokeLinecap="round" strokeLinejoin="round" d="M12 15a3 3 0 1 0 0-6 3 3 0 0 0 0 6Z" />
                    </svg>
                  )}
                </button>
              </div>
            </div>
            <Button className="w-full" type="submit">
              Entrar
            </Button>
            <div className="text-center pt-2">
              <a href="/signup" className="text-sm text-black hover:underline">Não tem conta? Cadastre-se</a>
            </div>
          </form>
        </div>
      </div>
    </div>
  ) : (
    <LoadingPage />
  );
}
