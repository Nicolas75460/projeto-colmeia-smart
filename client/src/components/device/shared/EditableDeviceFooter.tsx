import { Button } from "@/components/ui/button";
import { useFormContext } from "react-hook-form";
import { Spinner } from "./Spinner";

interface DeviceFooterProps {
  onCancel: () => void;
}

export function EditableDeviceFooter({ onCancel }: DeviceFooterProps) {
  const { formState: { isSubmitting, isValid, isDirty } } = useFormContext();

  return (
        <div className="flex gap-3 pt-10 justify-end">
    <Button
      disabled={isSubmitting}
      onClick={onCancel}
      variant="outline"
      className="w-32 bg-secondary py-2 rounded-md text-foreground"
    >
      Cancelar
    </Button>
     <Button
      type="submit"
      disabled={isSubmitting || !isValid || !isDirty}
      className="submit-button w-32 bg-primary text-secondary py-2 rounded-md"
    >
      {isSubmitting ? (
        <Spinner />
      ) : (
        "Salvar"
      )}
    </Button>
    </div>
  )

}